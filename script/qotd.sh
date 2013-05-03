<?php

    require( "simple_html_dom.php");
    $html = file_get_html("http://www.brainyquote.com/quotes_of_the_day.html");

   $element = $html->find("div.boxyPaddingBig");
   // foreach( $html->find("div.boxyPaddingBig") as $element) {
        print $element->find('div.bqQuoteLink',0)->plaintext;
    //}

?>
