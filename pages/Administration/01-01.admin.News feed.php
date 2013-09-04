<?php
//if(strpos($_SESSION['RIGHTS'],"admin")===FALSE) rss_display_array("$PHP_PATH/news.rss");
//else { require "modules/admin.inc"; admin_rss("$PHP_PATH/news.rss"); }
require_once "modules/rss.inc" ; admin_rss("$PHP_PATH/news.rss");
?>
