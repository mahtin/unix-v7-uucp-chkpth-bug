# The 1978 UNIX v7 UUCP chkpth() bug

Unix v7 UUCP had a bug in the `chkpth()` code

There was a bug in UUCP way-way-back in the late 70's and early 80's within the original UUCP code included in Unix v7 and it's newer derived versions.

## CVE-2021-41773
 
The `..` bug lives on in the recent [CVE-2021-41773](https://nvd.nist.gov/vuln/detail/CVE-2021-41773) (CVE == [Common Vulnerabilities and Exposures](https://en.wikipedia.org/wiki/Common_Vulnerabilities_and_Exposures)).
It's fully explained in Cloudflare's [article](https://blog.cloudflare.com/helping-apache-servers-stay-safe-from-zero-day-path-traversal-attacks/) from their blog.

## The UNIX v7 UUCP bug

The [chkpth()](https://github.com/v7unix/v7unix/blob/master/v7/usr/src/cmd/uucp/chkpth.c) function didn't check correctly for `//..//` in the path. This was not found till 1981 by [Peter Honeyman](https://eecs.engin.umich.edu/people/honeyman-peter/), who was at Bell Labs Murray Hill back in the early 80's.

The code in question was line 64-68.
```
	/*  check for /../ in path name  */
	for (s = path; *s != '\0'; s++) {
		if (*s == '/' && prefix("../", (++s)))
			return(FAIL);
	}
```

This code will allow `//..//` to pass thru undetected.

## Example code to show the vulnerability.

Included is some ssample to show the issue that existed in 1978-1981.

```
$ make 
cc    -c -o chkpth-test.o chkpth-test.c
cc   chkpth-test.o   -o chkpth-test
$ make run
./chkpth-test < test-chkpth-input.txt
HELLO-WORLD                                       	SUCCESS
/foo                                              	SUCCESS
/foo/                                             	SUCCESS
/foo/bar                                          	SUCCESS
/foo/bar/                                         	SUCCESS
/foo//bar                                         	SUCCESS
/foo//bar/                                        	SUCCESS
/foo//bar//                                       	SUCCESS
//foo                                             	SUCCESS
//foo/                                            	SUCCESS
//foo/bar                                         	SUCCESS
//foo/bar/                                        	SUCCESS

..                                                	SUCCESS
/..                                               	SUCCESS
/foo/..                                           	SUCCESS
/foo/../                                          	FAIL
/foo/bar/..                                       	SUCCESS
/foo/bar/../                                      	FAIL
../foo                                            	SUCCESS
../foo/                                           	SUCCESS
../foo/bar                                        	SUCCESS
../foo/bar/                                       	SUCCESS
/../foo                                           	FAIL
/../foo/                                          	FAIL
/../foo/bar                                       	FAIL
/../foo/bar/                                      	FAIL

//foo/bar/../                                     	FAIL
/..//foo/bar/                                     	FAIL
/..//foo/bar//                                    	FAIL
//../../foo/bar/                                  	FAIL
//..//../foo/bar/                                 	SUCCESS
//../foo/bar/                                     	SUCCESS
//../foo/bar/../                                  	FAIL

//..//..//..//..//..//..//etc/passwd              	SUCCESS
$
```

The final line being what Peter Honeyman realized worked quite successfully. I was the only system admin at Bell Labs that noticed this transfer from my machine and traced it back to him. Peter and I have been friends ever since.

You can plan with the `test-chkpth-input.txt` and provide any tests you require.

## The UUCP fix

Dan Nowitz rewrote this specific code to canonicalize the path instead of rejecting the `../`.

Later on, UUCP was 100% recoded from scratch and became something called **Honey DanBer UUCP**. It's written about in [HONEY DANBER - THE UUCP OF THE FUTURE.  P. Honeyman, D.A. Nowitz, Be Redman](https://www.bell-labs.com/institute/publications/bl8491475/) and is the surviving version of UUCP.

## Modern day networking

UUCP in-fact didn't fully survive.

## Summary

Please write better code.
