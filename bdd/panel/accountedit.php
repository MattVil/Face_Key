<?php
    include 'local.postgre.conf.php';
    // include 'postgresql.conf.inc.php';
    include 'fonction.php';

    $id = $_GET['id'];

    $thisaccount = "account.php?id=$id";
    $view = "Location: $thisaccount";
    $thisedit = "./accountedit.php?id=$id";
    $exist = false;

    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;

            if($k=="domain"){
                $pr = $_POST[$k];
                $new_id_site = get_info("Sites", $_POST[$k], "id_site", "domain");
                edit_table("account",$id,"id_site", $new_id_site, "id_account");  
 
            }
            else if($k=="tag"){
                $new_id_tag = get_info("Tags", $_POST[$k], "id_tag", "name_tag");
                edit_table("account",$id,"id_tag",$new_id_tag,"id_account");   

            }

            else{
                edit_table("account",$id,$k,$_POST[$k],"id_account");   
            }
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
    <a href="userview.php">Back</a><br />
    <a href="<?php echo $profile?> ">Back to User Profile</a><br />
    <a href="<?php echo $thisedit; ?>">Edit</a><br/>
    <a href="<?php echo $thisdelete; ?>">Delete</a>
    <br/>
    <br/>
    <form action="<?php $thisedit?>" method="post">
        <ul>
            <li> site : <select name="domain"><?php echo get_list("Sites","domain",$domain)?></select></li>
            <li> login : <input name="login" placeholder="<?php echo $login?>"/></li>
            <li> password : <input name="password" placeholder="<?php echo $password ?>"/></li>
            <li> tag : <select name="tag"><?php echo get_list("Tags","name_tag",$tag)?></select></li>
        </ul>
        <input type="submit" value="Submit">
    </form>
    </body>
</html>
