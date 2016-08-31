<!doctype html>
<html>
    <head>
        <title>Flag!</title>
        <meta charset="utf-8" />
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.7/css/materialize.min.css">
        <style type="text/css">
        html {
            height: 100%;
        }
        body {
            min-height: 100%;
            height: 100%;
        }
        .hello {
            font-size: 2em;
            width: 100%;
        }
        .hello-wrapper {
            height: 100%;
        }
        .hello p {
            color: #555;
        }
        </style>
    </head>
    <body>
        <div class="valign-wrapper hello-wrapper">
            <div class="hello valign center-align">
                <p>
                    <?php
                    echo @file_get_contents('flag.txt');
                    ?>
                </p>
            </div>
        </div>
    </body>
</html>
