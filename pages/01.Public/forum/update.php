<?php

require_once("filenames.php");
require_once("../../../modules/flat.inc");
require_once("../../../modules/rss.inc");
require_once("../../../modules/html.inc");

/*** load boarg given in URL ($_GET['BOARD'])or create a blank one in "$rss_board" ***/
$path_parts = pathinfo($_SERVER['SCRIPT_NAME']);
if(file_exists("{$_GET['BOARD']}/$RSS_INDEX")) $rss_board=rss_read_array("{$_GET['BOARD']}/$RSS_INDEX");
else $rss_board=array( array(	'TITLE'=>"Forum Title",
				'LINK'=>"http://{$_SERVER['HTTP_HOST']}{$path_parts['dirname']}/{$_GET['BOARD']}/$HTM_INDEX",
				'DESCRIPTION'=>"Forum Description")
			);

/*** modify $rss_thread and $rss_board if needed ***/
if(!empty($_POST['text'])) //If somebody pasted a new thread/reply
{  $date=date($DATE_FORMAT);
   $temp=array( 'TITLE'      => $_POST['object'],
		'DESCRIPTION'=> $_POST['text'], //csv_text_encode(),
		'PUBDATE'    => $date,
		'AUTHOR'     => "'{$_POST['name']}' {$_POST['mail']}",		//'LINK'       => $link
              );
   if(empty($_GET['THREAD'])) //This is a new thread
   {  $temp['LINK']="http://{$_SERVER['HTTP_HOST']}{$path_parts['dirname']}/{$_GET['BOARD']}/$HTM_INDEX";
      $temp['GUID']=$date.".htm";
      $temp['CATEGORY']=1;
      $rss_board[]=$temp;
   }
   else //this is a reply
   {  $rss_thread=rss_read_array("{$_GET['BOARD']}/{$_GET['THREAD']}.rss");
      $temp['LINK']="http://{$_SERVER['HTTP_HOST']}{$path_parts['dirname']}/{$_GET['BOARD']}/{$_GET['THREAD']}.htm";
      $temp['GUID']="{$rss_thread[0]['GUID']}".".htm";
      $rank=array_find($rss_board,$_GET['THREAD']);
      $rss_board[$rank]['CATEGORY']++;
      $rss_board[$rank]['PUBDATE']=$date;
   }
   $rss_thread[]=$temp;
   rss_write_array("{$_GET['BOARD']}/{$rss_thread[0]['GUID']}.rss", $rss_thread);
   rss_write_array("{$_GET['BOARD']}/$RSS_INDEX"                  , $rss_board);
   html_static_generation($PHP_THREADS, "{$_GET['BOARD']}/{$rss_thread[0]['GUID']}.htm");
   html_static_generation($PHP_INDEX,   "{$_GET['BOARD']}/{$HTM_INDEX}");
}
header ("Location: {$_GET['BOARD']}/$HTM_INDEX");
?>
