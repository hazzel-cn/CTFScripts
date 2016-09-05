<?php

/*
 * Bugs to be fixed:
    1. Sometimes GET prameter will not show in the page.
    2. Requests:get / post can not request with $header array.
*/





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
        $this->Headers = $headers;
        return $this->Headers;
    }


    function send_request()
    {
        $method = $_SERVER['REQUEST_METHOD'];
        $url = $this->url;
        $headers = $this->Headers;
        
        if ($method == 'GET')
        {
            print_r($headers);
            $request = Requests::get($url, array('X-Requests' => 'Is Awesome!'));
        }
        elseif ($method == 'POST') {
            $data = $this->POSTs;
            print_r($data);
            $request = Requests::post($url, $data);
        }
        echo $request->body;
        #echo "<br><br>Hi";
    }

}


$r = new reqPackage();
echo $r->get_url();
$r->load_header();
$r->load_data();
$r->send_request();

?>