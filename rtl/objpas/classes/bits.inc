{
    This file is part of the Free Component Library (FCL)
    Copyright (c) 1999-2008 by the Free Pascal development team

    See the file COPYING.FPC, included in this distribution,
    for details about the copyright.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

 **********************************************************************}
{****************************************************************************}
{*                               TBits                                      *}
{****************************************************************************}

Procedure BitsError (const Msg : string);

begin
  Raise EBitsError.Create(Msg) at get_caller_addr(get_frame);
end;

Procedure BitsErrorFmt (const Msg : string; const Args : array of const);

begin
  Raise EBitsError.CreateFmt(Msg,args) at get_caller_addr(get_frame);
end;

{Min function for Longint}
Function liMin(X, Y: Longint): Longint;
  begin
    Result := X;
    if X > Y then Result := Y;
  end;

procedure TBits.CheckBitIndex (Bit : longint;CurrentSize : Boolean);

begin
 if (bit<0) or (CurrentSize and (Bit >= FBSize)) then
   BitsErrorFmt(SErrInvalidBitIndex,[bit]);
 if (bit>=MaxBitFlags) then
   BitsErrorFmt(SErrIndexTooLarge,[bit])

end;

{ ************* functions to match TBits class ************* }

procedure TBits.setSize(value: longint);
var
  newSize, loop: LongInt;
begin
  CheckBitIndex(value, false);

  if value <> 0 then
    newSize :=  (value shr BITSHIFT) + 1
  else
    newSize := 0;

  if newSize <> FSize then
  begin
    ReAllocMem(FBits, newSize * SizeOf(longint));
    if FBits <> nil then
    begin
      if newSize > FSize then
        for loop := FSize to newSize - 1 do
          FBits^[loop] := 0;
    end
    else if newSize > 0 then
      BitsError(SErrOutOfMemory);  { isn't ReallocMem supposed to throw EOutOfMemory? }
    FSize := newSize;
  end;
  FBSize := value;
end;

procedure TBits.SetBit(bit : longint; value : Boolean);
var
  n: Integer;
begin
  grow(bit+1);   { validates bit range and adjusts FBSize if necessary }
  n := bit shr BITSHIFT;
  if value then
    FBits^[n] := FBits^[n] or (longword(1) shl (bit and MASK))
  else
    FBits^[n] := FBits^[n] and not (longword(1) shl (bit and MASK));
end;

function TBits.OpenBit : longint;
var
   loop : longint;
   loop2 : longint;
begin
   result := -1; {should only occur if the whole array is set}
   { map 0 to -1, 1..32 to 0, etc }
   for loop := 0 to ((FBSize + MASK) shr BITSHIFT) - 1 do
   begin
      if FBits^[loop] <> $FFFFFFFF then
      begin
         for loop2 := 0 to MASK do
         begin
           if (FBits^[loop] and (longint(1) shl loop2)) = 0 then
           begin
             result := (loop shl BITSHIFT) + loop2;
             if result > FBSize then
               result := FBSize;
             Exit;
           end;
         end;
      end;
   end;

   if FSize < MaxBitRec then
     result := FSize * 32;  {first bit of next record}
end;

{ ******************** TBits ***************************** }

constructor TBits.Create(theSize : longint = 0 );
begin
   FSize := 0;
   FBSize := 0;
   FBits := nil;
   findIndex := -1;
   findState := True;  { no reason just setting it to something }
   if TheSize > 0 then grow(theSize);
end;

destructor TBits.Destroy;
begin
   if FBits <> nil then
      FreeMem(FBits, FSize * SizeOf(longint));
   FBits := nil;

   inherited Destroy;
end;

procedure TBits.grow(nbit: longint);
begin
  if nbit > FBSize then
    SetSize(nbit);
end;

function TBits.getFSize : longint;
begin
   result := FSize;
end;

procedure TBits.seton(bit : longint);
begin
  SetBit(bit, True);
end;

procedure TBits.clear(bit : longint);
begin
  SetBit(bit, False);
end;

procedure TBits.clearall;
var
   loop : longint;
begin
   for loop := 0 to FSize - 1 do
      FBits^[loop] := 0;
{ don't clear FBSize here, it will cause exceptions on subsequent reading bit values }
{ use 'Size := 0' to reset everything and deallocate storage }
end;

function TBits.get(bit : longint) : Boolean;
var
   n : longint;
begin
   CheckBitIndex(bit,true);
   result := False;
   n := bit shr BITSHIFT;
   if (n < FSize) then
      result := (FBits^[n] and (longint(1) shl (bit and MASK))) <> 0;
end;

procedure TBits.andbits(bitset : TBits);
var
   n : longint;
   loop : longint;
begin
   if FSize < bitset.getFSize then
      n := FSize - 1
   else
      n := bitset.getFSize - 1;

   for loop := 0 to n do
      FBits^[loop] := FBits^[loop] and bitset.FBits^[loop];

   for loop := n + 1 to FSize - 1 do
      FBits^[loop] := 0;
end;

procedure TBits.notbits(bitset : TBits);
var
   n : longint;
   jj : cardinal;
   loop : longint;
begin
   if FSize < bitset.getFSize then
      n := FSize - 1
   else
      n := bitset.getFSize - 1;

   for loop := 0 to n do
   begin
      jj := FBits^[loop];
      FBits^[loop] := FBits^[loop] and (jj xor bitset.FBits^[loop]);
   end;
end;

procedure TBits.orbits(bitset : TBits);
var
   loop : longint;
begin
   if FBSize < bitset.Size then
     grow(bitset.Size);

   for loop := 0 to FSize-1 do
      FBits^[loop] := FBits^[loop] or bitset.FBits^[loop];
end;

procedure TBits.xorbits(bitset : TBits);
var
   loop : longint;
begin
   if FBSize < bitset.Size then
     grow(bitset.Size);

   for loop := 0 to FSize-1 do
      FBits^[loop] := FBits^[loop] xor bitset.FBits^[loop];
end;

function TBits.equals(bitset : TBits) : Boolean;
var
   n : longint;
   loop : longint;
begin
   result := False;

   if FSize < bitset.getFSize then
      n := FSize - 1
   else
      n := bitset.getFSize - 1;

   for loop := 0 to n do
      if FBits^[loop] <> bitset.FBits^[loop] then exit;

   if FSize - 1 > n then
   begin
      for loop := n to FSize - 1 do
         if FBits^[loop] <> 0 then exit;
   end
   else if bitset.getFSize - 1 > n then
      for loop := n to bitset.getFSize - 1 do
         if bitset.FBits^[loop] <> 0 then exit;

   result := True;  {passed all tests}
end;


{ us this in place of calling FindFirstBit. It sets the current }
{ index used by FindNextBit and FindPrevBit                     }

procedure TBits.SetIndex(index : longint);
begin
   CheckBitIndex(index,true);
   findIndex := index;
end;


{ When state is set to True it looks for bits that are turned On (1) }
{ and when it is set to False it looks for bits that are turned      }
{ off (0).                                                           }

function TBits.FindFirstBit(state : boolean) : longint;
var
   loop : longint;
   loop2 : longint;
   startIndex : longint;
   stopIndex : Longint;
   compareVal : cardinal;
begin
   result := -1; {should only occur if none are set}

   findState := state;

   if state = False then
      compareVal := $FFFFFFFF  { looking for off bits }
   else
      compareVal := $00000000; { looking for on bits }

   for loop := 0 to FSize - 1 do
   begin
      if FBits^[loop] <> compareVal then
      begin
         startIndex := loop * 32;
         stopIndex:= liMin(StartIndex+31,FBSize -1);
         for loop2 := startIndex to stopIndex do
         begin
            if get(loop2) = state then
            begin
               result := loop2;
               break; { use this as the index to return }
            end;
         end;
         break;  {stop looking for bit in records }
      end;
   end;

   findIndex := result;
end;

function TBits.FindNextBit : longint;
var
   loop : longint;
   maxVal : longint;
begin
   result := -1;  { will occur only if no other bits set to }
                  { current findState                        }

   if findIndex > -1 then { must have called FindFirstBit first }
   begin                  { or set the start index              }
      maxVal := (FSize * 32) - 1;

      for loop := findIndex + 1 to maxVal  do
      begin
         if get(loop) = findState then
         begin
            result := loop;
            break;
         end;
      end;

      findIndex := result;
   end;
end;

function TBits.FindPrevBit : longint;
var
   loop : longint;
begin
   result := -1;  { will occur only if no other bits set to }
                  { current findState                        }

   if findIndex > -1 then { must have called FindFirstBit first }
   begin                  { or set the start index              }
      for loop := findIndex - 1 downto 0  do
      begin
         if get(loop) = findState then
         begin
            result := loop;
            break;
         end;
      end;

      findIndex := result;
   end;
end;


