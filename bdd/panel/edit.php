<?php
    // include 'local.postgre.conf.php';
    include 'postgresql.conf.inc.php';
    include 'fonction.php';

    $id = $_GET['id'];

    $view = "Location: userview.php";
    $thisedit = "/edit.php?id=$id";
    $profile = "/profile.php?id=$id";
    $exist = false;

    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;
            edit_table("users",$id,$k,$_POST[$k],"id_user");
        }
    }
    if($exist){
        header($view);
    }

    $name = get_info("users", $id, "name", "id_user");
    $first_name = get_info("users", $id, "first_name", "id_user");
    $pseudo = get_info("users", $id, "pseudo", "id_user");
    $gender = get_info("users", $id, "gender", "id_user");
    $mail =  get_info("users", $id, "mail", "id_user");
    $FKpass = get_info("users", $id, "password", "id_user");
    $creation_date = get_info("users", $id, "creation_date", "id_user");
    $language = get_info("users", $id, "language", "id_user");
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
    <a href="userview.php">Back</a><br/>
    <a href="<?php echo $profile ?>">Profile</a>
    <br/>
    <br/>
    <h1>Edit of <?php echo $pseudo?></h1>
    <form action="<?php $thisedit ?>" method="post">

        <ul>
            <li> name : <input type="text" name="name" placeholder="<?php echo $name ?>"/></li>
            <li> first Name : <input type="text" name="first_name" placeholder="<?php echo $first_name ?>"/></li>
            <li> pseudo : <input type="text" name="pseudo" placeholder="<?php echo $pseudo ?>"/></li>
            <li> gender : <input type="text" name="gender" placeholder="<?php echo $gender  ?>"/></li>
            <li> mail : <input type="text" name="mail" placeholder="<?php echo $mail ?>"/></li>
            <li> Face Key password : <input type="text" name="password" placeholder="<?php echo $FKpass ?>"/></li>
            <li> creation date : <input type="text" name="creation_date" placeholder="<?php echo $creation_date ?>"/></li>
            <li> language : <input type="text" name="language" placeholder="<?php echo $language ?>"/> </li>
        </ul>
        <input type="submit" value="Submit">
    </form>

    </body>
</html>
