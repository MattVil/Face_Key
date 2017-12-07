<?php
    // include 'local.postgre.conf.php';
    include 'postgresql.conf.inc.php';
    include 'fonction.php';

    $id = $_GET['id'];


    $view = "Location: userview.php";
    $thisedit = "./accountedit.php?id=$id";
    $thisdelete = "./deleteacount.php?id=$id";
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
    $name_user = get_info("Users", $id_user, "pseudo", "id_user");
    
    $profile = "./profile.php?id=$id_user";


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
    <div class="center">
        <a href="userview.php">Back</a>
        <a href="<?php echo $profile?> ">Back to User Profile</a>
        <a href="<?php echo $thisedit; ?>">Edit</a>
        <a href="<?php echo $thisdelete; ?>">Delete</a>
    </div>

    <h1><?php echo $domain ?> account of <?php echo $name_user ?></h1>
    <h2>Info</h2>
        <ul>
            <li> site : <?php echo $domain?></li>
            <li> login : <?php echo $login?></li>
            <li> password : <?php echo $password ?></li>
            <li> tag : <?php echo $tag?></li>
        </ul>
    <h2>Shard with</h2>
    <?php 
        $query = "SELECT id_sharedAccount, Users.id_user, Users.name, Users.first_name, Users.pseudo, SharedAccount.expiration_date FROM SharedAccount INNER JOIN Users ON Users.id_user = SharedAccount.id_receiver WHERE SharedAccount.id_account = $id;";
        echo display_table_query($query,5);
    ?>
    </body>
</html>
