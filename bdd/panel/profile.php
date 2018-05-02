<?php
    include "include.php";

    include ('lib/jpgraph/src/jpgraph.php');
    include ('lib/jpgraph/src/jpgraph_bar.php');


    $id = $_GET['id'];
    $edit = "./edit.php?id=$id";
    $delete = "./delete.php?id=$id";
    $imgpath = "./users/$id/graphFreq.png";

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
                                    <a href="<?php echo $edit ?>" class="gradient-22 nava">Edit</a>
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

            <h2 class="gradient-8"><?php echo $pseudo?>'s profile</h2>
            <div class="row">
                <div class="col-12 col-sm-12">
                    <h3>Informations.</h3>
                	<ul class="center">
                		<li> <strong>name :</strong> <?php echo $name ?></li>
                		<li>  <strong>first Name :</strong> <?php echo $first_name ?></li>
                		<li>  <strong>pseudo :</strong> <?php echo $pseudo ?></li>
                		<li>  <strong>gender :</strong> <?php echo $gender ?></li>
                		<li>  <strong>mail :</strong> <?php echo $mail ?></li>
                		<li>  <strong>Face Key password :</strong> <?php echo $FKpass ?></li>
                		<li>  <strong>creation date :</strong> <?php echo $creation_date ?></li>
                		<li>  <strong>language :</strong> <?php echo $language ?></li>
                  </ul>
                </div>
            </div>
            <div class="row">
                <h3>Accounts.</h3>
                    <div class="center"><a class="gradient-2" href="addaccount.php?id=<?php echo $id?>">Add new Account ></a></div>
                    <br/>
                    <div class="tablediv">
                        <?php echo display_table_query("SELECT account.id_account, domain, account.login, account.password FROM Account INNER JOIN Sites ON account.id_site = sites.id_site
                      LEFT JOIN PaymentAccount ON account.id_account = paymentaccount.id_account
                    WHERE account.id_user = $id AND paymentaccount.id_account IS NULL;", 1); ?>
                    </div>
                    <div class="tablediv">
                    <?php echo display_table_query("SELECT id_account, domain, login, password, bank, rib, card_num, cryptogram FROM PaymentAccount INNER JOIN Sites ON paymentaccount.id_site = sites.id_site
                    WHERE paymentaccount.id_user = $id;", 2); ?>
                </div>
            </div>
            <div class="row">
        		<h3>Accounts shared with <?php echo get_info("users", $id, "pseudo","id_user") ?>.</h3>
        		<div class="tablediv">
                    <?php
                        $query = "SELECT sharedAccount.id_account, domain, name, first_name, SharedAccount.expiration_date FROM SharedAccount INNER JOIN Account ON sharedAccount.id_account = account.id_account INNER JOIN Sites ON account.id_site = sites.id_site INNER JOIN Users ON account.id_user = users.id_user WHERE sharedAccount.id_receiver = $id";
                        echo display_table_query($query,6); 
                    ?>
                </div>
            </div>
        </div>
    </body>
</html>
