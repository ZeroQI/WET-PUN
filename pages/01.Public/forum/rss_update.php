<?php	include "../../../modules/flat.inc";
	include "../../../modules/html.inc";

/*** load boards,threads rss array ***/
$path_parts = pathinfo($_SERVER['SCRIPT_NAME']);
if(file_exists($_GET['BOARD']."/board.rss")) $rss_board=rss_read_array($_GET['BOARD']."/board.rss");
else $rss_board=array( array(	'TITLE'=>"Forum Title",
				'LINK'=>"http://{$_SERVER['HTTP_HOST']}{$path_parts['dirname']}/{$_GET['BOARD']}/board.htm",
				'DESCRIPTION'=>"Forum Description")
			);
if(!empty($_GET['THREAD'])) $rss_thread=rss_read_array("{$_GET['BOARD']}/{$_GET['THREAD']}.rss");

/*** modify array in memory ***/
if(!empty($_POST['text']))
{  $date=date("Y-m-d D, H\hi");
   if(empty($_GET['THREAD']))   $link="http://{$_SERVER['HTTP_HOST']}{$path_parts['dirname']}/{$_GET['BOARD']}/board.htm";
   else                         $link="http://{$_SERVER['HTTP_HOST']}{$path_parts['dirname']}/{$_GET['BOARD']}/{$_GET['THREAD']}.htm";
   $temp=array( 'TITLE'      => $_POST['object'],
		'DESCRIPTION'=> CSV_text_encode($_POST['text']),
		'PUBDATE'    => $date,
		'AUTHOR'     => "'{$_POST['name']}' {$_POST['mail']}",
		'LINK'       => $link
              );
   if(empty($_GET['THREAD']))
   {  $temp['GUID']=$date;
      $temp['CATEGORY']=1;
      $rss_board[]=$temp;
   }
   else
   {  $rank=array_find($rss_board,$_GET['THREAD']);
      $rss_board[$rank]['CATEGORY']++;
      $rss_board[$rank]['PUBDATE']=$date;
   }
   $rss_thread[]=$temp;
   rss_write_array("{$_GET['BOARD']}/{$rss_thread[0]['GUID']}.rss", $rss_thread);
   html_static_generation("rss_template_threads.php", "{$_GET['BOARD']}/".(empty($_GET['THREAD'])?$date:$rss_thread[0]['GUID']).".htm");
}
rss_write_array($_GET['BOARD']."/board.rss", $rss_board );
html_static_generation("rss_template_index.php", "{$_GET['BOARD']}/board.htm");
header ("Location: {$_GET['BOARD']}/board.htm");
?>
