<?php
    include "include.php";

    $id = $_GET['id'];

    $view = "Location: userview.php";
    $thisedit = "./edit.php?id=$id";
    $profile = "./profile.php?id=$id";
    $exist = false;
    $delete = "./delete.php?id=$id";

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
                                    <!-- <a href="<?php // echo $delete ?>" class="gradient-22 nava">Delete</a> -->
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
            <h2 class="gradient-8">Edit of <?php echo $pseudo?></h2>
            <form action="<?php $thisedit ?>" method="post">
                <ul>
                    <li> <strong>name :</strong> <input type="text" name="name" placeholder="<?php echo $name ?>"/></li>
                    <li> <strong>first Name :</strong> <input type="text" name="first_name" placeholder="<?php echo $first_name ?>"/></li>
                    <li> <strong>pseudo :</strong> <input type="text" name="pseudo" placeholder="<?php echo $pseudo ?>"/></li>
                    <li> <strong>gender :</strong> <input type="text" name="gender" placeholder="<?php echo $gender  ?>"/></li>
                    <li> <strong>mail :</strong> <input type="text" name="mail" placeholder="<?php echo $mail ?>"/></li>
                    <li> <strong>Face Key password :</strong> <input type="text" name="password" placeholder="<?php echo $FKpass ?>"/></li>
                    <li> <strong>creation date :</strong> <input type="text" name="creation_date" placeholder="<?php echo $creation_date ?>"/></li>
                    <li> <strong>language :</strong> <input type="text" name="language" placeholder="<?php echo $language ?>"/> </li>
                </ul>
                <div class="center"><input type="submit" value="Submit"></div>
            </form>
        </div>
    </body>
</html>
