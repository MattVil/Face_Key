<?php
    include 'local.postgre.conf.php';
    // include 'postgresql.conf.inc.php';
    include 'fonction.php';

    include ('lib/jpgraph/src/jpgraph.php');
    include ('lib/jpgraph/src/jpgraph_bar.php');


    $id = $_GET['id'];
    $edit = "./edit.php?id=$id";
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
    <a href="userview.php">Back</a>
    <br/>
    <a href="<?php echo $edit ?>">Edit</a>
	<br/><br/>

	<ul>
		<li> name : <?php echo $name ?></li>
		<li> first Name : <?php echo $first_name ?></li>
		<li> pseudo : <?php echo $pseudo ?></li>
		<li> gender : <?php echo $gender ?></li>
		<li> mail : <?php echo $mail ?></li>
		<li> Face Key password : <?php echo $FKpass ?></li>
		<li> creation date : <?php echo $creation_date ?></li>
		<li> language : <?php echo $language ?></li>
  </ul>


		<h2>CO Proprietaire</h2>
		<?php echo display_table_query("SELECT account.id_account, domain, account.login, account.password FROM Account INNER JOIN Sites ON account.id_site = sites.id_site
      LEFT JOIN PaymentAccount ON account.id_account = paymentaccount.id_account
    WHERE account.id_user = $id AND paymentaccount.id_account IS NULL;", 1); ?>
    <?php echo display_table_query("SELECT id_account, domain, login, password, bank, rib, card_num, cryptogram FROM PaymentAccount INNER JOIN Sites ON paymentaccount.id_site = sites.id_site
    WHERE paymentaccount.id_user = $id;", 2); ?>

		<h2>CO partagée avec <?php echo get_info("users", $id, "pseudo") ?></h2>
		<?php echo display_table_query("SELECT id_sharedAccount, domain, name, first_name FROM SharedAccount INNER JOIN Account
      ON sharedAccount.id_account = account.id_account INNER JOIN Sites ON account.id_site = sites.id_site INNER JOIN Users ON account.id_user = users.id_user
      WHERE sharedAccount.id_receiver = $id"); ?>
    <h2>Graph</h2>
		<?php createGraph($id);?>
		<img src="<?php echo $imgpath ?>" alt="graphFreq"/>

    



    </body>
</html>
