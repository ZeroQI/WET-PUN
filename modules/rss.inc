<?php

/*** Load RSS file into array ***/
function startElementHandler($parser, $name, $a) { global $tag,$count;      if($name=="ITEM") $count++; elseif($name!='RSS' && $name!='CHANNEL') $tag=$name; }
function         DataHandler($parser, $data)     { global $tag,$count,$RSS; if(trim($data)) { if(empty($RSS[$count][$tag])) $RSS[$count][$tag]=""; $RSS[$count][$tag].=str_replace("\t","\r\n",htmlentities($data)); } }
function   endElementHandler($parser, $name)     { global $tag,$count;      $tag=""; }

function rss_read_array($URL)
{  global $RSS, $count;
   $count=0; $RSS=array(); $xml_parser=xml_parser_create();
   xml_set_element_handler($xml_parser, "startElementHandler","endElementHandler");
   xml_set_character_data_handler($xml_parser, "DataHandler");
   xml_parse($xml_parser, implode( "\t", file($URL)), TRUE);
   xml_parser_free($xml_parser);
   return ($RSS);
}

/*** write an RSS file from array ***/
function rss_write_array($FILENAME, $RSS)
{  $STRING ="<?xml version='1.0' encoding='utf-8'?>\r\n".
            "<rss version='0.91' xml:base='???'>\r\n".
            "<channel>\r\n";
   $ITEM=array_shift($RSS);
   foreach ($ITEM as $ID => $VALUE) { $ID=strtolower($ID); $STRING.="   <$ID>$VALUE</$ID>\r\n"; }
   foreach ($RSS as $NULL => $ITEM)
   {  $STRING.="\r\n<item>\r\n";
      foreach ($ITEM as $ID => $VALUE)
      {  $ID=strtolower($ID);
         $STRING.="<$ID>".htmlentities($VALUE)."</$ID>\r\n";
      }
      $STRING.="</item>\r\n";
   }
   $STRING.="\r\n</channel>\r\n</rss>";
   csv_write_array($FILENAME, array($STRING));
}

/*** Display en RSS array ***/
function rss_display_array($URL)
{  if(!file_exists($URL))
   {  $RSS=array( array( 'TITLE'  =>"News feed", 'LINK'       =>"pages/news.rss",
                         'AUTHOR' =>"ZeroQI", 'DESCRIPTION'=>"Newsfeed for the site" ) );
      rss_write_array($URL, $RSS);
   }
   else $RSS=rss_read_array($URL); next($RSS);
   
   foreach ($RSS as $LINE_KEY => $LINE)
   {  if(empty($LINE['AUTHOR'])) $LINE['AUTHOR']="Author Unknown";
      //if(!$LINE_KEY) article("<A HREF='{$LINE['LINK']}'>{$LINE['TITLE']}</A>",$LINE['DESCRIPTION']);
      if($LINE_KEY)
      //else
      {  article("<a href=\"{$LINE['LINK']}\">".$LINE['TITLE']."</a> - {$LINE['AUTHOR']} - {$LINE['PUBDATE']}");
         echo str_replace("\r\n","<br />",html_entity_decode($LINE['DESCRIPTION']));
      }
      article();
}  }

/*** Manages RSS messages ***/
function admin_rss($URL)
{  if(!isset($_POST['CONTENT'])) $_POST['CONTENT']=""; 
   if(!isset($_GET['ACTION']))   $_GET['ACTION']=""; 
   if(!isset($_GET['ID']))       $_GET['ID']="";
   $ARRAY=rss_read_array($URL);
   if( $_GET['ACTION']=="remove" || $_GET['ACTION']=="add" || $_GET['ACTION']=="update" ) /*** Array modification ***/
   {  if($_GET['ACTION']=="remove") array_splice($ARRAY,$_GET['ID'],1);
      if($_GET['ACTION']=="update") array_splice($ARRAY,$_GET['ID'],1,array($_POST['data']));
      if($_GET['ACTION']=="add"   ) array_splice($ARRAY,1,0,array($_POST['data']));
      rss_write_array($URL, $ARRAY);
   }
   echo "<table>\n<tr> <th>Title, Link, Author, Date</th> <th width='85%'>Description</th> <th width='15%'>Action</th> </tr>\n";
   if($_GET['ACTION']!="modify") $ARRAY[]="ADD";
   foreach ($ARRAY as $LINE_KEY => $LINE)
   {  if($LINE!="ADD" and !isset($LINE['PUBDATE']))  $LINE['PUBDATE']="";
      if($LINE!="ADD" and !isset($LINE['AUTHOR']))  $LINE['AUTHOR']="";
      //print_r($LINE);
	  echo "<tr class='content' valign='top'>";
      
	  if($LINE!="ADD" && ($_GET['ACTION']!="modify" || $LINE_KEY!=$_GET['ID'] ) )  /*** display row ***/
      {  $PREVIEW=substr( strtr( strip_tags(   rtrim(str_replace( "\r\r\n", "\n", $LINE['DESCRIPTION'])) ), "\n", " "), 0, 110)." ...";
         echo " <td>{$LINE['TITLE']}<br />{$LINE['LINK']}<br />{$LINE['AUTHOR']}<br />{$LINE['PUBDATE']}</td>".
              " <td>&nbsp;$PREVIEW</td> <td ALIGN='CENTER'> <a href='index.php?page={$_GET['page']}&ACTION=modify&ID=$LINE_KEY'>Modifier</a>&nbsp;".
              " <a HREF='index.php?page={$_GET['page']}&ACTION=remove&ID=$LINE_KEY' onclick='if( !confirm('Delete news {$LINE['TITLE']}') ) {location.href='#';return false;}'>Delete</A></td>";
      }
      
	  else /*** add & modify form ***/
      {  if($LINE=="ADD") $temp=array("add", "Title", "http://{$_SERVER['HTTP_HOST']}/index.php", "Description", "Creator", gmdate('D, d M Y H:i:s \G\M\T',time()) );
         else             $temp=array("update", $LINE['TITLE'], $LINE['LINK'], $LINE['DESCRIPTION'], $LINE['AUTHOR'] , $LINE['PUBDATE']);
         echo "<form method='POST' action='index.php?page={$_GET['page']}&ACTION={$temp[0]}&ID={$_GET['ID']}'>\n".
		      "<td valign='top' width='0%'>".
		      "<input type='text' name='data[TITLE]'  value='{$temp[1]}' size='30'><br /> <input type='text' name='data[LINK]'    value='{$temp[2]}' size=30><br />".
              "<input type='text' name='data[AUTHOR]' value='{$temp[4]}' size='30'><br /> <input type='text' name='data[PUBDATE]' value='{$temp[5]}' size=30> </td>".
              "<td><textarea wrap='off' name='data[DESCRIPTION]' cols='60%' rows='4'>{$temp[3]}</textarea></td>".
              "<td><input type='submit' value='{$temp[0]}'>".($LINE=="ADD" ? " " : " <a HREF='index.php?page={$_GET['page']}'>Cancel</A>")."</td></form>";
      }
      echo "</tr>\n";
    }
   echo "</table><br /><br />\n";
}
?>
