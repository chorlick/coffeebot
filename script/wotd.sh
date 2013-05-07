<?php

    $element = simplexml_load_file("http://dictionary.reference.com/wordoftheday/wotd.rss");

    //print_r($element);


    printf("%s\n", $element->channel->item->description);
    printf("Brought to you by %s\n", $element->channel->title);    



	

?>
