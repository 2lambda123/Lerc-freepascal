{ CoreGraphics - CGPDFContentStream.h
 * Copyright (c) 2004 Apple Computer, Inc.
 * All rights reserved.
 }
{       Pascal Translation:  Peter N Lewis, <peter@stairways.com.au>, August 2005 }
{
    Modified for use with Free Pascal
    Version 200
    Please report any bugs to <gpc@microbizz.nl>
}

{$mode macpas}
{$packenum 1}
{$macro on}
{$inline on}
{$CALLING MWPASCAL}

unit CGPDFContentStream;
interface
{$setc UNIVERSAL_INTERFACES_VERSION := $0342}
{$setc GAP_INTERFACES_VERSION := $0200}

{$ifc not defined USE_CFSTR_CONSTANT_MACROS}
    {$setc USE_CFSTR_CONSTANT_MACROS := TRUE}
{$endc}

{$ifc defined CPUPOWERPC and defined CPUI386}
	{$error Conflicting initial definitions for CPUPOWERPC and CPUI386}
{$endc}
{$ifc defined FPC_BIG_ENDIAN and defined FPC_LITTLE_ENDIAN}
	{$error Conflicting initial definitions for FPC_BIG_ENDIAN and FPC_LITTLE_ENDIAN}
{$endc}

{$ifc not defined __ppc__ and defined CPUPOWERPC}
	{$setc __ppc__ := 1}
{$elsec}
	{$setc __ppc__ := 0}
{$endc}
{$ifc not defined __i386__ and defined CPUI386}
	{$setc __i386__ := 1}
{$elsec}
	{$setc __i386__ := 0}
{$endc}

{$ifc defined __ppc__ and __ppc__ and defined __i386__ and __i386__}
	{$error Conflicting definitions for __ppc__ and __i386__}
{$endc}

{$ifc defined __ppc__ and __ppc__}
	{$setc TARGET_CPU_PPC := TRUE}
	{$setc TARGET_CPU_X86 := FALSE}
{$elifc defined __i386__ and __i386__}
	{$setc TARGET_CPU_PPC := FALSE}
	{$setc TARGET_CPU_X86 := TRUE}
{$elsec}
	{$error Neither __ppc__ nor __i386__ is defined.}
{$endc}
{$setc TARGET_CPU_PPC_64 := FALSE}

{$ifc defined FPC_BIG_ENDIAN}
	{$setc TARGET_RT_BIG_ENDIAN := TRUE}
	{$setc TARGET_RT_LITTLE_ENDIAN := FALSE}
{$elifc defined FPC_LITTLE_ENDIAN}
	{$setc TARGET_RT_BIG_ENDIAN := FALSE}
	{$setc TARGET_RT_LITTLE_ENDIAN := TRUE}
{$elsec}
	{$error Neither FPC_BIG_ENDIAN nor FPC_LITTLE_ENDIAN are defined.}
{$endc}
{$setc ACCESSOR_CALLS_ARE_FUNCTIONS := TRUE}
{$setc CALL_NOT_IN_CARBON := FALSE}
{$setc OLDROUTINENAMES := FALSE}
{$setc OPAQUE_TOOLBOX_STRUCTS := TRUE}
{$setc OPAQUE_UPP_TYPES := TRUE}
{$setc OTCARBONAPPLICATION := TRUE}
{$setc OTKERNEL := FALSE}
{$setc PM_USE_SESSION_APIS := TRUE}
{$setc TARGET_API_MAC_CARBON := TRUE}
{$setc TARGET_API_MAC_OS8 := FALSE}
{$setc TARGET_API_MAC_OSX := TRUE}
{$setc TARGET_CARBON := TRUE}
{$setc TARGET_CPU_68K := FALSE}
{$setc TARGET_CPU_MIPS := FALSE}
{$setc TARGET_CPU_SPARC := FALSE}
{$setc TARGET_OS_MAC := TRUE}
{$setc TARGET_OS_UNIX := FALSE}
{$setc TARGET_OS_WIN32 := FALSE}
{$setc TARGET_RT_MAC_68881 := FALSE}
{$setc TARGET_RT_MAC_CFM := FALSE}
{$setc TARGET_RT_MAC_MACHO := TRUE}
{$setc TYPED_FUNCTION_POINTERS := TRUE}
{$setc TYPE_BOOL := FALSE}
{$setc TYPE_EXTENDED := FALSE}
{$setc TYPE_LONGLONG := TRUE}
uses MacTypes,CFArray,CGPDFObject,CGBase;
{$ALIGN POWER}


// CGPDFContentStreamRef defined in CGBase


{ Create a content stream from `page'. }

function CGPDFContentStreamCreateWithPage( page: CGPDFPageRef ): CGPDFContentStreamRef; external name '_CGPDFContentStreamCreateWithPage'; (* AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER *)

{ Create a content stream from `stream'. }

function CGPDFContentStreamCreateWithStream( stream: CGPDFStreamRef; streamResources: CGPDFDictionaryRef; parent: CGPDFContentStreamRef ): CGPDFContentStreamRef; external name '_CGPDFContentStreamCreateWithStream'; (* AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER *)

{ Increment the retain count of `cs'. }

function CGPDFContentStreamRetain( cs: CGPDFContentStreamRef ): CGPDFContentStreamRef; external name '_CGPDFContentStreamRetain'; (* AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER *)

{ Decrement the retain count of `cs'. }

procedure CGPDFContentStreamRelease( cs: CGPDFContentStreamRef ); external name '_CGPDFContentStreamRelease'; (* AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER *)

{ Return the array of CGPDFStreamRefs comprising the entire content stream
 * of `cs'. }

function CGPDFContentStreamGetStreams( cs: CGPDFContentStreamRef ): CFArrayRef; external name '_CGPDFContentStreamGetStreams'; (* AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER *)

{ Return the resource named `name' in category `category' of the resource
 * dictionaries of `cs'. }

function CGPDFContentStreamGetResource( cs: CGPDFContentStreamRef; category: ConstCStringPtr; name: ConstCStringPtr ): CGPDFObjectRef; external name '_CGPDFContentStreamGetResource'; (* AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER *)


end.