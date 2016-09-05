<?php

include 'Requests/library/Requests.php';
Requests::register_autoloader();
#echo "Good<br>";
# you can include this php script.

/**
* This class is for the request object
*/
class reqPackage
{
    var $domin = 'http://www.hazzel.cn';     # Domin to send request
    var $Port = '80';      # PORT
    var $POSTs;     # POST array
    var $Headers;   # Header information
    var $Cookies;   # Cookies


    function get_url() 
    {
        if (isset($_SERVER['REQUEST_URI']))
        {
            $uri = $_SERVER['REQUEST_URI'];
        }
        else 
        {
            if (isset($_SERVER['argv'])) 
            {
                $uri = $_SERVER['PHP_SELF'] .'?'. $_SERVER['argv'][0];
            }
            else 
            {
                $uri = $_SERVER['PHP_SELF'] .'?'. $_SERVER['QUERY_STRING'];
            }
        }
        $this->url = $this->domin . $uri;
        return $this->url;
    }


    function load_data()
    {
        $this->POSTs = $_POST;
        return $this->POSTs;
    }


    function load_header()
    {
        $headers = array();
        /*
        foreach ($_SERVER as $key => $value) {
            if ('HTTP_' == substr($key, 0, 5)) {
                $headers[str_replace('_', '-', substr($key, 5))] = $value;
            }
        }
        */
        $headers = getallheaders();
        $this->headers = $headers;
        return $this->headers;
    }


    function send_request()
    {
        $method = $_SERVER['REQUEST_METHOD'];
        #$url = $this->url;
        $url = 'http://www.hazzel.cn';
        $headers = $this->headers;
        if ($method == 'GET')
        {
            var_dump($headers);
            $request = Requests::get($url, $headers);
        }
        elseif ($method == 'POST') {
            #$request = Requests::post($this->url, $this->headers, $this->POSTs);
            echo '1';
        }
        echo $request->body;
        #echo "<br><br>Hi";
    }

}


$r = new reqPackage();
$r->get_url();
$r->load_header();
$r->send_request();

?>