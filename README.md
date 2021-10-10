# The 1978 UNIX v7 UUCP chkpth() bug

Unix v7 [UUCP](https://en.wikipedia.org/wiki/UUCP) had a bug in the `chkpth()` code.
The bug in UUCP is from way-way-back in the late 70's and early 80's and was within the original UUCP code included in Unix v7 and it's newer derived versions.

## CVE-2021-41773
 
The `..` bug lives on in [Apache](https://www.apache.org) and the recent [CVE-2021-41773](https://nvd.nist.gov/vuln/detail/CVE-2021-41773) (CVE == [Common Vulnerabilities and Exposures](https://en.wikipedia.org/wiki/Common_Vulnerabilities_and_Exposures)).
It's also fully explained in Cloudflare's [blog post](https://blog.cloudflare.com/helping-apache-servers-stay-safe-from-zero-day-path-traversal-attacks/) from their company blog. In this case, the `%2e` character is used for a `.` in the file path.

## The UNIX v7 UUCP bug

The [chkpth()](https://github.com/v7unix/v7unix/blob/master/v7/usr/src/cmd/uucp/chkpth.c) function didn't check correctly for `//..//` in the path. This was found till 1981 by [Peter Honeyman](https://eecs.engin.umich.edu/people/honeyman-peter/), who was at Bell Labs Murray Hill back in the early 80's.

The code in question is line 64-68.
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

The final line being what Peter Honeyman realized worked quite successfully. During his test, I was the only system admin at Bell Labs that noticed this transfer in my logs. I traced it back to him. Peter and I have been friends ever since.

You can play with the `test-chkpth-input.txt` file and provide any tests you require.

## The UUCP fix

Dave Nowitz (known as `dan` because that was his login) rewrote this specific code to canonicalize the path instead of rejecting the `../`.

Later on, UUCP was rewritten (some parts recoded from scratch, some left alone - like [g-protocol](https://en.wikipedia.org/wiki/UUCP#g-protocol)). This new effort became something called **Honey DanBer UUCP**. It's written about in [HONEY DANBER - THE UUCP OF THE FUTURE. Peter Honeyman, Dave A Nowitz, Brian E Redman](https://www.bell-labs.com/institute/publications/bl8491475/) and is the surviving version of UUCP.

The new version was presented via:

- NOWITZ, D.A., HONEYMAN, P., AND REDMAN, B.E. 1984. Experimental implementation of UUCP—security aspects. In Proceedings of the USENIX UniForum Conference (Washington, D.C., January 17–20, 1984). USENIX Association, Berkeley, CA, 245–250.
- HONEYMAN, P., NOWITZ, D.A., AND REDMAN, B.E. 1984. New Protection Features in UUCP. UNIFORUM (January 1984).
- HONEYMAN, P., NOWITZ, D.A., AND REDMAN, B.E. 1984. An Experimental Version of UUCP. UNIFORUM (January 1984).
- HONEYMAN, P., NOWITZ, D.A., AND REDMAN, B.E. 1984. Honey DanBer – The UUCP of the Future. European UNIX Systems User Group Spring Meeting (April 1984).

But there aren't any copies online.

## Modern day networking

In fact, UUCP didn't fully survive.

## Summary

Please write better code.
