    StartPackage('pcap');
    {$IF defined(ALLPACKAGES)}
    Directory:='extra/pcap';
    {$ELSEIF defined(EXTRAPACKAGES)}
    Directory:='pcp';
    {$ENDIF}
    OS:=[linux,netbsd,freebsd,openbsd,win32,solaris];
    T:=Targets.AddUnit('pcap');
    EndPackage;