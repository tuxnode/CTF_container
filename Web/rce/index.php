<?php
    highlight_file(__FILE__);

    $target = $_GET['ip'];

    if(isset($target)){
        $blacklist = array(" ", "ls", "cat", "flag");
        foreach($blacklist as $word){
            if(preg_match("/$word/", $target)){
                die("Hack detected! No '$word' allowed.");
            }
        }

        // 执行命令
        // 原意：ping -c 4 127.0.0.1
        echo "<pre>";
        system("ping -c 4 " . $target);
        echo "</pre>";
    }
?>
