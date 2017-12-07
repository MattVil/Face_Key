<?php
    // include 'local.postgre.conf.php';
    include 'postgresql.conf.inc.php';
    include 'fonction.php';

    $id = $_GET['id'];


    $view = "Location: userview.php";
    $thisedit = "/accountedit.php?id=$id";
    $exist = false;

    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;
            edit_table("account",$id,$k,$_POST[$k]);
        }
    }
    if($exist){
        header($view);
    }

    $id_site = get_info("account", $id, "id_site", "id_account");
    $domain = get_info("Sites", $id_site, "domain", "id_site");
    $login = get_info("account", $id, "login", "id_account");
    $password = get_info("account", $id, "password", "id_account") ;
    $id_tag = get_info("account", $id, "id_tag", "id_account");
    $tag = get_info("Tags", $id_tag, "name_tag", "id_tag");
    $id_user = get_info("account", $id, "id_user", "id_account");
    
    $profile = "/profile.php?id=$id_user";


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
    <a href="userview.php">Back</a><br />
    <a href="<?php echo $profile?> ">Back to User Profile</a><br />
    <a href="<?php echo $thisedit; ?>">Edit</a><br/>
    <a href="<?php echo $thisdelete; ?>">Delete</a>
    <br/>
    <br/>

        <ul>
            <li> site : <?php echo $domain?></li>
            <li> login : <?php echo $login?></li>
            <li> password : <?php echo $password ?></li>
            <li> tag : <?php echo $tag?></li>
        </ul>

    </body>
</html>
