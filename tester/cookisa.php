<?php

    session_start();

    $_SESSION["name"] = "zarafox";

    $cookie_name = "achrafix";
    $cookie_value = "valuedzab";
    setcookie("achrafix", "valuedzab", time() + (86400 * 30), "/"); // 86400 = 1 day


    echo "Session and cookies has been set    ";


?>