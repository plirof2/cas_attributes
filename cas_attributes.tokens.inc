<?php

/**
 * @file
 * Token module integration.
 */

/**
 * Implements hook_token_info_alter().
 */
function cas_attributes_token_info_alter(&$data) {
  if (function_exists('cas_phpcas_attributes')) {
    $data['tokens']['cas']['attribute'] = array(
      'name' => t('Attribute'),
      'description' => t('A CAS attribute of the user. (Only available if user is logged into CAS). Always stored as an array token (thus supporting multivalue attributes); therefore be sure to remember to specify which array member you want (for instance, to get the first value: [cas:attribute:?:first]).  <a href="@url">Available tokens</a>.', array('@url' => url('admin/config/people/cas/attributes/cas'))),
      'dynamic' => TRUE,
    );
  }
}

/**
 * Implements hook_tokens().
 */
function cas_attributes_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();

  if ($type == 'cas' && !empty($data['cas'])) {
    $cas = $data['cas'];

    // Provide [cas:attribute:?] dynamic tokens.
    if ($attribute_tokens = token_find_with_prefix($tokens, 'attribute')) {
      $attribute = array_change_key_case(cas_phpcas_attributes($cas));
      foreach ($attribute_tokens as $name => $original) {
        // If there are no options specified have it return all values.
        if (strpos($name, ':') === FALSE) {
          $name .= ':join';
        }

        // Break out the token into attributes and the options for them.
        list($name, $token) = explode(':', $name, 2);

        $name = drupal_strtolower($name);
        if (isset($attribute[$name])) {
          $value = $attribute[$name];
          if (!is_array($value)) {
            $value = array($value);
          }
          $replacements += token_generate('array', array($token => $original), array('array' => $value), $options);
        }
        elseif ($name == '?') {
          $keys = array_keys($attribute);
          if ($sanitize) {
            $keys = array_map('check_plain', $keys);
          }
          $replacements[$original] = t('Available attributes: %keys', array('%keys' => implode(', ', $keys)));
        }
      }
    }
    $replacements = add_fixed_tokens($attribute,$replacements,$options,$token); //added by Jon 2015
  }

  return $replacements;
}


//+++++++++Added by J +++++++++++++++++++++++++++++++++++++++
function add_fixed_tokens($attribute,$replacements,$options,$token){

$debug_me=false;
 if($debug_me)  drupal_set_message("HARDCODED  :  <pre> edupersonorgunitdn:gsnunitcode:extended = ".print_r($attribute['edupersonorgunitdn:gsnunitcode:extended'], true)." </pre>");   //debug JON 161104
 if($debug_me)  drupal_set_message("HARDCODED  :  <pre> edupersonorgunitdn:gsnunitcode = ".print_r($attribute['edupersonorgunitdn:gsnunitcode'], true)." </pre>");   //debug JON 161104

          //jon note !!! $original is [current-user:cas:attribute:edupersonorgunitdn:cn:extended]  , $name=edupersonorgunitdn:cn:extended
          //jon note : should change $original to eg [current-user:cas:attribute:edupersonorgunitdn-cn-extended]

$original= '[current-user:cas:attribute:edupersonorgunitdn-gsnunitcode-extended]';
$value=$attribute['edupersonorgunitdn:gsnunitcode:extended'];
if (!is_array($value)) {            $value = array($value);          }
$replacements += token_generate('array', array($token => $original), array('array' => $value), $options);


//try to get schcode (Ypepth code)
$original= '[current-user:cas:attribute:schcode]';
$value=$attribute['edupersonorgunitdn:gsnunitcode:extended'];
$exploded_value=explode($value);
$value=end($exploded_value); //JON : ASSUME that the last item is the SCH CODE/YPEPETH CODE we want
if (!is_array($value)) {            $value = array($value);          }
$replacements += token_generate('array', array($token => $original), array('array' => $value), $options);


$original= '[current-user:cas:attribute:edupersonorgunitdn-gsnunitcode]';
$value=$attribute['edupersonorgunitdn:gsnunitcode'];
if (!is_array($value)) {            $value = array($value);          }
$replacements += token_generate('array', array($token => $original), array('array' => $value), $options);


$original= '[current-user:cas:attribute:edupersonorgunitdn-cn-extended]';
$value=$attribute['edupersonorgunitdn:cn:extended'];
if (!is_array($value)) {            $value = array($value);          }
$replacements += token_generate('array', array($token => $original), array('array' => $value), $options);



if($debug_me)  drupal_set_message("add_fixed_tokens replacements : ".'<pre>'. print_r($replacements, true) .'</pre>');   //debug JON 161104
 return $replacements;

}

//---------------------------------------------