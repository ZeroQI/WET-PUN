<?php global $rss_board, $rss_thread;

require("filenames.php");
if(empty($rss_board))
{  $_GET['BOARD']="test";
   $DYNAMIC="";
   include_once "../../../modules/rss.inc";
   $rss_board =rss_read_array("{$_GET['BOARD']}/{$RSS_INDEX}");
   $rss_thread=rss_read_array("{$_GET['BOARD']}/{$rss_thread[0]['GUID']}.rss");
}
else $DYNAMIC="../"; //html generated link,$rss_board array allready created
                 
$TEMPLATE_VAR=array("%TPL_CSS%"     => "{$DYNAMIC}{$TPL_CSS}",
                    "%RSS_THREADS%" => "{$DYNAMIC}{$_GET['BOARD']}/{$rss_thread[0]['GUID']}.rss",
                    "%PHP_POST%"    => "{$DYNAMIC}{$PHP_POST}?BOARD={$_GET['BOARD']}&THREAD={$rss_thread[0]['GUID']}&OBJECT={$rss_thread[0]['TITLE']}"
                   );
$ARRAY=explode("<!--loop-->", str_replace( array_keys($TEMPLATE_VAR), $TEMPLATE_VAR, implode("", file($TPL_THREADS)) ));unset($TEMPLATE_VAR);
$count=count($ARRAY);$loop=0;$OUTPUT="";//array_shift($rss_board);

foreach($rss_thread as $LINE_NB=>&$FIELD)
{  $TEMPLATE_VAR=array( "%TITLE%"       => $FIELD['TITLE'],
                        "%DESCRIPTION%" => $FIELD['DESCRIPTION'],
                        "%AUTHOR%"      => $FIELD['AUTHOR'],
                        "%PUBDATE%"     => $FIELD['PUBDATE'],
                      );
   $OUTPUT.=str_replace( array_keys($TEMPLATE_VAR), $TEMPLATE_VAR, $ARRAY[$loop=$loop%($count-2)+1] );
}
echo $ARRAY[0].$OUTPUT.$ARRAY[$count-1];
?>
