<?php
    include "include.php";

    $id = $_GET['id'];

    $thisaccount = "account.php?id=$id";
    $view = "Location: $thisaccount";
    $thisedit = "./accountedit.php?id=$id";
    $thisdelete = "./deleteacount.php?id=$id";
    $check = "./account.php?id=$id";
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
        <div class="container">
            <div class="row vcenter nav-back">
            </div>
            <div class="row nav">
                <div class="container">
                    <div class="row vcenter">
                        <div class="col-6 ">
                            <div class="row">
                                <h1 class="left"><a class="logo" href="./index.php">Facekey Admin Panel</a></h1>
                            </div>
                        </div>
                        <div class="col-6 right">
                            <div class="row vcenter">
                                <div class="">
                                </div>
                                <div class="col-8">
                                    <a href="./profile.php?id=1 " class="gradient-22 nava">Profile</a>
                                    <a href="./account.php?id=1" class="gradient-22 nava">Check</a>
                                    <a href="./deleteacount.php?id=1" class="gradient-22 nava">Delete</a>
                                </div>
                                <div class="col-4">
                                    <a href="./userview.php" class="nava white">Users </a>
                                    <a href="./siteview.php" class="nava white">Sites </a>

                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="nav-post">
            </div>

            <h2 class="gradient-8">Edit <?php echo $domain ?> account of <?php echo $name_user ?></h2>            
            <form action="<?php $thisedit?>" method="post">
                <ul>
                    <li> <strong>site :</strong> <select name="domain"><?php echo get_list("Sites","domain",$domain)?></select></li>
                    <li> <strong>login :</strong> <input name="login" placeholder="<?php echo $login?>"/></li>
                    <li> <strong>password :</strong> <input name="password" placeholder="<?php echo $password ?>"/></li>
                    <li> <strong>tag :</strong> <select name="tag"><?php echo get_list("Tags","name_tag",$tag)?></select></li>
                </ul>
                <div class="center">
                    <input type="submit" value="Submit">
                </div>
            </form>
        </div>
    </body>
</html>
