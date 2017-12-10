<?php
    include "include.php";


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
        <div class="container">
            <div class="row vcenter">
                <div class="col-6 ">
                  <h1 class="gradient-4"><a href="./index.php" style="font-weight: 900;">Facekey Admin Panel</a></h1>
                </div>
                <div class="col-6 right">
                    <a href="userview.php" class="gradient-2">Back</a>
                </div>
            </div>
            <h2 class="gradient-1">Add account</h2>
            <form action="./add.php" method="post">
                <ul>
                    <li> <strong>name :</strong> <input type="text" name="name"/></li>
                    <li> <strong>first Name :</strong> <input type="text" name="first_name"/></li>
                    <li> <strong>pseudo :</strong> <input type="text" name="pseudo" /></li>
                    <li> <strong>gender :</strong> <input type="text" name="gender"/></li>
                    <li> <strong>mail :</strong> <input type="text" name="mail" /></li>
                    <li> <strong>password :</strong> <input type="text" name="password" /></li>
                    <li> <strong>creation date :</strong> <input type="text" name="creation_date"/></li>
                    <li> <strong>language :</strong> <input type="text" name="language" /> </li>
                </ul>
                <div class="center">
                <input type="submit" value="Submit">
                </div>
            </form>
        </div>
    </body>
</html>
