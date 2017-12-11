<?php
    include "include.php";
    
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
                                    <a href="<?php echo $profile ?>" class="gradient-22 nava">Profile</a>
                                    <a href="<?php echo $thisedit ?>" class="gradient-22 nava">Edit</a>
                                    <a href="<?php echo $thisdelete ?>" class="gradient-22 nava">Delete</a>
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
            <h2 class="gradient-8"><?php echo $domain ?> account of <?php echo $name_user ?></h2>
            <div class="row">
                <div class="col-6 col-sm-12">
                    <h3>Informations.</h3>
                    <ul>
                        <li> <strong>site :</strong> <?php echo $domain?></li>
                        <li> <strong>login :</strong> <?php echo $login?></li>
                        <li> <strong>password :</strong> <?php echo $password ?></li>
                        <li> <strong>tag :</strong> <?php echo $tag?></li>
                    </ul>
                </div>
                <div class="col-6 col-sm-12">
                    <h3>Share.</h3>
                    <div class="center"><a href="./newshare.php?id=<?php echo $id?>" class="gradient-2">Share connexion to user ></a></div>
                </div>
            </div>
            <h3>Shared with.</h3>
            <div class="tablediv">
                <?php 
                    $query = "SELECT id_sharedAccount, Users.id_user, Users.name, Users.first_name, Users.pseudo, SharedAccount.expiration_date FROM SharedAccount INNER JOIN Users ON Users.id_user = SharedAccount.id_receiver WHERE SharedAccount.id_account = $id;";
                    echo display_table_query($query,5);
                ?>
            </div>
        </div>
    </body>
</html>
