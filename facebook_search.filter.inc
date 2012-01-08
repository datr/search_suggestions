<?php

function filter_xss($string, $allowed_tags = array('a', 'em', 'strong', 'cite', 'code', 'ul', 'ol', 'li', 'dl', 'dt', 'dd')) {
  // Only operate on valid UTF-8 strings. This is necessary to prevent cross
  // site scripting issues on Internet Explorer 6.
  if (!drupal_validate_utf8($string)) {
    return '';
  }
  // Store the input format
  _filter_xss_split($allowed_tags, TRUE);
  // Remove NUL characters (ignored by some browsers)
  $string = str_replace(chr(0), '', $string);
  // Remove Netscape 4 JS entities
  $string = preg_replace('%&\s*\{[^}]*(\}\s*;?|$)%', '', $string);

  // Defuse all HTML entities
  $string = str_replace('&', '&amp;', $string);
  // Change back only well-formed entities in our whitelist
  // Named entities
  $string = preg_replace('/&amp;([A-Za-z][A-Za-z0-9]*;)/', '&\1', $string);
  // Decimal numeric entities
  $string = preg_replace('/&amp;#([0-9]+;)/', '&#\1', $string);
  // Hexadecimal numeric entities
  $string = preg_replace('/&amp;#[Xx]0*((?:[0-9A-Fa-f]{2})+;)/', '&#x\1', $string);

  return preg_replace_callback('%
    (
    <(?=[^a-zA-Z!/])  # a lone <
    |                 # or
    <[^>]*(>|$)       # a string that starts with a <, up until the > or the end of the string
    |                 # or
    >                 # just a >
    )%x', '_filter_xss_split', $string);
}

function _filter_xss_split($m, $store = FALSE) {
  static $allowed_html;

  if ($store) {
    $allowed_html = array_flip($m);
    return;
  }

  $string = $m[1];

  if (substr($string, 0, 1) != '<') {
    // We matched a lone ">" character
    return '&gt;';
  }
  else if (strlen($string) == 1) {
    // We matched a lone "<" character
    return '&lt;';
  }

  if (!preg_match('%^(?:<\s*(/\s*)?([a-zA-Z0-9]+)([^>]*)>?|(<!--.*?-->))$%', $string, $matches)) {
    // Seriously malformed
    return '';
  }

  $slash = trim($matches[1]);
  $elem = &$matches[2];
  $attrlist = &$matches[3];
  $comment = &$matches[4];

  if ($comment) {
    $elem = '!--';
  }

  if (!isset($allowed_html[strtolower($elem)])) {
    // Disallowed HTML element
    return '';
  }

  if ($comment) {
    return $comment;
  }

  if ($slash != '') {
    return "</$elem>";
  }

  // Is there a closing XHTML slash at the end of the attributes?
  // In PHP 5.1.0+ we could count the changes, currently we need a separate match
  $xhtml_slash = preg_match('%\s?/\s*$%', $attrlist) ? ' /' : '';
  $attrlist = preg_replace('%(\s?)/\s*$%', '\1', $attrlist);

  // Clean up attributes
  $attr2 = implode(' ', _filter_xss_attributes($attrlist));
  $attr2 = preg_replace('/[<>]/', '', $attr2);
  $attr2 = strlen($attr2) ? ' ' . $attr2 : '';

  return "<$elem$attr2$xhtml_slash>";
}