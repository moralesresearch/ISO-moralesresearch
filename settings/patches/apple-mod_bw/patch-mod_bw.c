Apache 2.4 support for Apple's mod_bw 0.8 (apache_mod_bw-205): client addresses and
host names come from the request (useragent_addr, ap_get_useragent_host), so they
agree with mod_remoteip; and a request without a User-Agent header no longer crashes
the child process when a BandWidth u:... rule is set

--- mod_bw.c.orig
+++ mod_bw.c
@@ -512,14 +512,14 @@
         switch (e[i].type) {
         case T_AGENT:
             uastr = apr_table_get(r->headers_in, "User-Agent");
-            if (e[i].agent && ap_regexec(e[i].agent, uastr, 0, NULL, 0)==0 ) 
+            if (e[i].agent && uastr && ap_regexec(e[i].agent, uastr, 0, NULL, 0)==0 ) 
                 return (e[i].rate);
             break;
         case T_ALL:
             return e[i].rate;
 
         case T_IP:
-            if (apr_ipsubnet_test(e[i].x.ip, r->connection->remote_addr)) {
+            if (apr_ipsubnet_test(e[i].x.ip, r->useragent_addr)) {
                 return e[i].rate;
             }
             break;
@@ -527,7 +527,7 @@
             if (!gothost) {
                 int remotehost_is_ip;
 
-                remotehost = ap_get_remote_host(r->connection, r->per_dir_config,
+                remotehost = ap_get_useragent_host(r,
                                                 REMOTE_DOUBLE_REV, &remotehost_is_ip);
 
                 if ((remotehost == NULL) || remotehost_is_ip)
@@ -601,14 +601,14 @@
         switch (e[i].type) {
         case T_AGENT:
             uastr = apr_table_get(r->headers_in, "User-Agent");
-            if (e[i].agent && ap_regexec(e[i].agent, uastr, 0, NULL, 0)==0 )
+            if (e[i].agent && uastr && ap_regexec(e[i].agent, uastr, 0, NULL, 0)==0 )
                 return (e[i].max);
             break;
         case T_ALL:
             return e[i].max;
 
         case T_IP:
-            if (apr_ipsubnet_test(e[i].x.ip, r->connection->remote_addr)) {
+            if (apr_ipsubnet_test(e[i].x.ip, r->useragent_addr)) {
                 return e[i].max;
             }
             break;
@@ -616,7 +616,7 @@
             if (!gothost) {
                 int remotehost_is_ip;
 
-                remotehost = ap_get_remote_host(r->connection, r->per_dir_config,
+                remotehost = ap_get_useragent_host(r,
                                                 REMOTE_DOUBLE_REV, &remotehost_is_ip);
 
                 if ((remotehost == NULL) || remotehost_is_ip)
@@ -644,7 +644,7 @@
     const char *uastr;
 
     remotehost =
-        ap_get_remote_host(r->connection, r->per_dir_config, REMOTE_HOST,
+        ap_get_useragent_host(r, REMOTE_HOST,
                            NULL);
 
     for (i = 0; i < a->nelts; i++) {
@@ -652,14 +652,14 @@
         switch (e[i].type) {
         case T_AGENT:
             uastr = apr_table_get(r->headers_in, "User-Agent");
-            if (e[i].agent && ap_regexec(e[i].agent, uastr, 0, NULL, 0)==0 )
+            if (e[i].agent && uastr && ap_regexec(e[i].agent, uastr, 0, NULL, 0)==0 )
                 return (e[i].sid);
             break;
         case T_ALL:
             return e[i].sid;
 
         case T_IP:
-            if (apr_ipsubnet_test(e[i].x.ip, r->connection->remote_addr)) {
+            if (apr_ipsubnet_test(e[i].x.ip, r->useragent_addr)) {
                 return e[i].sid;
             }
             break;
@@ -667,7 +667,7 @@
             if (!gothost) {
                 int remotehost_is_ip;
 
-                remotehost = ap_get_remote_host(r->connection, r->per_dir_config,
+                remotehost = ap_get_useragent_host(r,
                                                 REMOTE_DOUBLE_REV, &remotehost_is_ip);
 
                 if ((remotehost == NULL) || remotehost_is_ip)
