<?php
require_once "modules/rss.inc"; rss_display_array("$PHP_PATH/news.rss");
article("Bennerie Generator");$ARRAY=file("$PHP_PATH/quotes.csv"); echo $ARRAY[array_rand($ARRAY)];
?>
