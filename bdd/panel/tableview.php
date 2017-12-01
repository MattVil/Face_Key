<?php
    if (isset($_GET["dbname"])){
    }else{
    echo "<script>javascript:history.go(-1)</script>";
    }
    include 'local.postgre.conf.php';
    //include 'postgresql.conf.inc.php'; 
    include 'fonction.php';
?>


<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        
        <link href="https://fonts.googleapis.com/css?family=Open+Sans:300,400,600" rel="stylesheet">
        <link rel="stylesheet" href="css/main.css">
        <link rel="shortcut icon" type="image/png" href="img/favicon.png">
        
        <title>Facekey &mdash; Admin Panel</title>
    </head>
    <body>
        <div class="container">
            <header class="header">
                <img src="img/logo.png" alt="trillo logo" class="logo">

                <form action="#" class="search">
                    <input type="text" class="search__input" placeholder="Search user">
                    <button class="search__button">
                        <svg class="search__icon">
                            <use xlink:href="img/sprite.svg#icon-magnifying-glass"></use>
                        </svg>
                    </button>
                </form>

                <nav class="user-nav">
                   <!-- <div class="user-nav__icon-box">
                        <svg class="user-nav__icon">
                            <use xlink:href="img/sprite.svg#icon-bookmark"></use>
                        </svg>
                        <span class="user-nav__notification">7</span>
                    </div>
                    -<div class="user-nav__icon-box">
                        <svg class="user-nav__icon">
                            <use xlink:href="img/sprite.svg#icon-chat"></use>
                        </svg>
                        <span class="user-nav__notification">13</span>
                    </div>
                    <div class="user-nav__user">
                        <img src="img/user.jpg" alt="User photo" class="user-nav__user-photo">
                        <span class="user-nav__user-name">Jonas</span>
                    </div> -->
                </nav>
            </header>


            <div class="content">
                <nav class="sidebar">
                    <ul class="side-nav">
                        <li class="side-nav__item">
                            <a href="index.php" class="side-nav__link">
                                <svg class="side-nav__icon">
                                    <use xlink:href="img/sprite.svg#icon-home"></use>
                                </svg>
                                <span>Home</span>
                            </a>
                        </li>
                        <li class="side-nav__item side-nav__item--active">
                            <a href="#" class="side-nav__link">
                                <svg class="side-nav__icon">
                                    <use xlink:href="img/sprite.svg#icon-pie"></use>
                                </svg>
                                <span><?php echo $_GET["dbname"]; ?></span>
                            </a>
                        </li>
                        <li class="side-nav__item">
                            <a href="#" class="side-nav__link">
                                <svg class="side-nav__icon">
                                    <use xlink:href="img/SVG/table.svg"></use>
                                </svg>
                                <span>Sites</span>
                            </a>
                        </li>
                        <li class="side-nav__item">
                            <a href="#" class="side-nav__link">
                                <svg class="side-nav__icon">
                                    <use xlink:href="img/SVG/pie-chart.svg"></use>
                                </svg>
                                <span>Stats</span>
                            </a>
                        </li>
                    </ul>

                    <div class="legal">
                        &copy; 2017 by FaceKey. All rights reserved.
                    </div>
                </nav>



               	 <main class="db-view">
                    <div class="overview">
                        <h1 class="overview__heading">
                            Welcome to <?php echo $_GET["dbname"]; ?> Panel 
                        </h1>
                        
                    </div>
                    <div class="detail">
                        <div class="description">
                            <p class="paragraph"> 
                                Here are datas for <?php echo $_GET["dbname"]; ?> table
                            </p>
                            <div style="overflow-x:auto;">
                                <?php
                                    echo display_table($_GET["dbname"]);
                                ?>
                            </div>
                        </div>
                    </div>
                </main>
            </div>
        </div>
    </body>
</html>