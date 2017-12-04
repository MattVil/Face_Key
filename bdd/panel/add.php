<?php
    include 'local.postgre.conf.php';
    //include 'postgresql.conf.inc.php'; 
    include 'fonction.php';

    
    $view = "Location: add.php";

    $champ = "";
    $value = "";

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
    <a href="userview.php">Back</a><br/><br/>
    <form action="/add.php" method="post">

        <ul>
            <li> name : <input type="text" name="name" placeholder="<?php echo get_info("users", $id, "name") ?>"/></li>
            <li> first Name : <input type="text" name="first_name"/></li>
            <li> pseudo : <input type="text" name="pseudo" /></li>
            <li> gender : <input type="text" name="gender"/></li>
            <li> mail : <input type="text" name="mail" /></li>
            <li> password : <input type="text" name="password" /></li>
            <li> creation date : <input type="text" name="creation_date"/></li>
            <li> language : <input type="text" name="language" /> </li>
        </ul> 
        <input type="submit" value="Submit">
    </form>
    
    </body>
</html>