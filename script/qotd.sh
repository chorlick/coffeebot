<?php

	require( "simple_html_dom.php");
	$html = file_get_html("http://www.brainyquote.com/quotes_of_the_day.html");

	$element = $html->find("div.boxyPaddingBig",0);
	$buffer = sprintf("%s\n\t-%s\n",$element->find('div.bqQuoteLink',0)->plaintext, $element->find('a',0)->plaintext);

	print $buffer;
?>
