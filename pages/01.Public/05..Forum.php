<?php article ( "R.I.S.C. Forum" ) ; ?>

Forum de discussion générant les pages en HTML 'Statique' lors de l'envoi.<br>
La consultation en html pur et dur est donc instantanée<br>
<br>
RISC est l'acronyme de (Really Implemented Statical Consultation).<br>
<br>
<?php
$HANDLE=opendir("$SECTION/forum");
while($FILE=readdir($HANDLE))
{  if(is_dir("$SECTION/forum/$FILE") && $FILE!="." && $FILE!=".." )
   {  if(is_file("$SECTION/forum/$FILE/board.htm"))
      {  echo "<A HREF='$SECTION/forum/$FILE/board.htm'>Statical</A> or ";
         echo "<A HREF='$SECTION/forum/index.php?BOARD=$FILE'>Dynamical</A> '$FILE' forum<br>\n";
      }
      elseif(strpos($_SESSION['RIGHTS'],"admin")!==FALSE) echo"<a href='$SECTION/forum/rss_update.php?BOARD=$FILE'>Create forum $FILE?</a><br/>\n";
  }  }
?>
Add database [to add with textbox+form]<br><br>

La partie design est calquée de FouleTexte 1.1 - (c) 2000 Thierry Arsicaud (deltascripts@ifrance.com)<br/>
