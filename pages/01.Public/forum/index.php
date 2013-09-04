<?php global $rss_board; 
require("filenames.php");

if(empty($rss_board))
{  $_GET['BOARD']="test";
   $DYNAMIC="";
   require_once "../../../modules/rss.inc";
   $rss_board=rss_read_array("{$_GET['BOARD']}/$RSS_INDEX");
}
else $DYNAMIC="../"; //html generated link,$rss_board array allready created

$TEMPLATE_VAR=array("%TPL_CSS%"   => "{$DYNAMIC}{$TPL_CSS}",
                    "%RSS_INDEX%" => "{$DYNAMIC}{$_GET['BOARD']}/{$RSS_INDEX}",
                    "%PHP_POST%"  => "{$DYNAMIC}{$PHP_POST}?BOARD={$_GET['BOARD']}"
                   );
$ARRAY=explode("<!--loop-->", str_replace( array_keys($TEMPLATE_VAR), $TEMPLATE_VAR, implode("", file("$TPL_INDEX")) ));unset($TEMPLATE_VAR);

$count=count($ARRAY);$loop=0;$OUTPUT="";array_shift($rss_board);
foreach($rss_board as $LINE_NB=>&$FIELD)
{  $TEMPLATE_VAR=array( "%ICON%"     => $FIELD['CATEGORY']<5?"/icons/small/dir.gif":"/icons//small/burst.gif",
                        "%TITLE%"    => $FIELD['TITLE'],
                        "%AUTHOR%"   => $FIELD['AUTHOR'],
                        "%PUBDATE%"  => $FIELD['PUBDATE'],
                        "%CATEGORY%" => $FIELD['CATEGORY']-1,
                        "%LINK%"     => empty($DYNAMIC)?"$PHP_THREADS?BOARD={$_GET['BOARD']}&THREAD={$FIELD['GUID']}":"{$FIELD['GUID']}.htm"
                      );
   $OUTPUT.=str_replace( array_keys($TEMPLATE_VAR), $TEMPLATE_VAR, $ARRAY[$loop=$loop%($count-2)+1] );
}
echo $ARRAY[0].$OUTPUT.$ARRAY[$count-1];
?>
