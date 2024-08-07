{
   This file is part of the Free Pascal run time library.
   (c) 2008 by Jonas Maebe
   member of the Free Pascal development team.

   See the file COPYING.FPC, included in this distribution,
   for details about the copyright.

   Signalhandler for Darwin/arm

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
}

procedure SignalToRunerror(Sig: cint; info : PSigInfo; SigContext:PSigContext); public name '_FPC_DEFAULTSIGHANDLER'; cdecl;

var
  res : word;

begin
  res:=0;
  case sig of
    SIGFPE :
      begin
        Case Info^.si_code Of
          FPE_FLTDIV,
          FPE_INTDIV : Res:=200;  { floating point divide by zero }
          FPE_FLTOVF : Res:=205;  { floating point overflow }
          FPE_FLTUND : Res:=206;  { floating point underflow }
          FPE_FLTRES,             { floating point inexact result }
          FPE_FLTINV : Res:=207;  { invalid floating point operation }
          Else
            Res:=207; {coprocessor error}
        end;
        SigContext^.uc_mcontext^.__fs.__fpscr := SigContext^.uc_mcontext^.__fs.__fpscr and not($1fff);
      end;
    SIGBUS:
        res:=214;
    SIGILL,
    SIGSEGV :
        res:=216;
    SIGINT:
        res:=217;
    SIGQUIT:
        res:=233;
  end;
  {$ifdef FPC_USE_SIGPROCMASK}
   reenable_signal(sig);
  {$endif }

  { return to trampoline }
  if res <> 0 then
    begin
      SigContext^.uc_mcontext^.__ss.__r[0] := res;
      SigContext^.uc_mcontext^.__ss.__r[1] := SigContext^.uc_mcontext^.__ss.__pc;
      SigContext^.uc_mcontext^.__ss.__r[2] := SigContext^.uc_mcontext^.__ss.__sp;
      pointer(SigContext^.uc_mcontext^.__ss.__pc) := @HandleErrorAddrFrame;
    end;
end;

