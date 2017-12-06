<?php
    //include 'local.postgre.conf.php';
    include 'postgresql.conf.inc.php';
    include 'fonction.php';


    $view = "Location: userview.php";

    $champ = "";
    $value = "";
    $exist = false;
    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;
            $champ = insert_request_champ($k,$champ);
            $value = insert_request_value($_POST[$k], $value);
            /*echo "ch : " . $champ . "<br>";
            echo "val : " . $value . "<br>";
            echo "<br>";*/
        }
    }
    if($exist){
        //echo "<br><br><br><br>final : <br>champs = " . $champ ."<br>value = " . $value . "<br>";
        add_to_table("users",$champ,$value);
        header($view);
    }
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <link rel="stylesheet" href="css/main.css">
        <link rel="shortcut icon" type="image/png" href="img/favicon.png">

        <title>Facekey &mdash; Admin Panel</title>
    </head>
    <body>
    <a href="siteview.php">Back</a><br/><br/>
    <form action="./siteview.php" method="post">

        <ul>
            <li> Domain : <input type="text" name="domain"/></li>
            <li> login_input : <input type="text" name="login"/></li>
            <li> password_input : <input type="text" name="password" /></li>
        </ul>
        <input type="submit" value="Submit">
    </form>

    <?php
      if (isset($_POST['domain']) || isset($_POST['login']) || isset($_POST['password'])){
        $query = "INSERT INTO Sites (domain, login_input, password_input) VALUES ('".$_POST['domain']."', '".$_POST['login']."', '".$_POST['password']."');";
        pg_query($query);
      }
     ?>

    </body>
</html>
