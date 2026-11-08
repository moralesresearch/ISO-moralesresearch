--- Src/signals.c.orig	2022-05-08 06:18:22 UTC
+++ Src/signals.c
@@ -703,5 +703,7 @@
 		errflag = noerrs = 0;
 		zwarn("timeout");
+		if (zleactive)
+			settyinfo(&shttyinfo);
 		stopmsg = 1;
 		zexit(SIGALRM, ZEXIT_SIGNAL);
 	    }
