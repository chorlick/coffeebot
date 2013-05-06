<?php

    require( "simple_html_dom.php");
    $html = file_get_html("http://www.brainyquote.com/quotes_of_the_day.html");

    $element = $html->find("div.boxyPaddingBig",0);
    $buffer = sprintf("%s\n\t-%s\n",$element->find('div.bqQuoteLink',0)->plaintext, $element->find('a',0)->plaintext);


    $line = "";
    $c = '';
    $i = 0;
    $pfile = fopen("../config.txt", "r+");
    if($pfile) {
        while(!feof($pfile)) {
           $c = fgetc($pfile);
           $line .= $c;
           
            if($c == "\n" ) {
              
                if(strstr( $line, "[qotd]")) {
                    printf("Found context\n");
                    printf("Adding : %s\n" , $buffer);
                    fprintf($pfile,"%s", $buffer);
                }else {
                        //do nothing
                }
                $line = "";
                $i = 0;
            }
        }
        fclose($pfile);
    }else {
        printf("Unable to open file");
    }


?>
