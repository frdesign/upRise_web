<?php

/**
 * @file
 * TBThemeFeed handles rss feed
 */
module_load_include('inc', 'tb_theme_feed', 'generator/field_generator');
module_load_include('inc', 'tb_theme_feed', 'generator/node_generator');

class TBThemeFeed {

  protected $configs;

  public function setConfig($cfg) {
    $this->configs = $cfg;
  }

  public function getConfig() {
    return $this->configs;
  }

  public function getType() {
    return "All";
  }

  public function crawlPage($url) {
    if (function_exists('curl_init')) {
      $curl = curl_init();
      curl_setopt($curl, CURLOPT_URL, $url);
      curl_setopt($curl, CURLOPT_HEADER, 0);
      curl_setopt($curl, CURLOPT_TIMEOUT, 600);
      curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($curl, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.5) Gecko/20041107 Firefox/1.0');
      $content = curl_exec($curl);
      curl_close($curl);
    }
    else {
      $content = @file_get_contents($url);
    }
    return $content;
  }

  public function getContent() {
    return NULL;
  }

  public function parseData($content) {
    return NULL;
  }

  public function feed() {
    set_time_limit(3600);
    $results = $this->getContent();
    $nodes = array();
    if (!is_array($results))
      return $nodes;
    foreach ($results as $row) {
      $existed = db_select('tb_theme_feed_contents', 'c')
        ->fields("c")
        ->condition('feed_id', $row['feed_id'])
        ->condition('account', $row['account'])
        ->condition('service', $row['service'])
        ->execute()
        ->fetchObject();
      if ($existed) {
        if ($row['changed'] > $existed->changed) {
          $node = tb_theme_feed_generate_node($row, $existed->node_id);
          $row_contents = array(
            'feed_id' => $row['feed_id'],
            'data' => serialize($row),
            'service' => $row['service'],
            'account' => $row['account'],
            'created' => $row['created'],
            'changed' => $row['changed'],
            'url' => $row['url'],
            'reference_url' => $row['reference_url'],
            'latest_feed' => mktime(),
            'node_id' => $node->nid,
          );
          db_update('tb_theme_feed_contents')
            ->fields($row_contents)
            ->condition('id', $existed->id)
            ->execute();
          $nodes[] = $node;
        }
      }
      else {
        $row['field_tb_wall_badge'] = 'tb-wall-new-badge';
        $node = tb_theme_feed_generate_node($row);
        $row_contents = array(
          'feed_id' => $row['feed_id'],
          'data' => serialize($row),
          'service' => $row['service'],
          'account' => $row['account'],
          'created' => $row['created'],
          'changed' => $row['changed'],
          'url' => $row['url'],
          'reference_url' => $row['reference_url'],
          'latest_feed' => mktime(),
          'node_id' => $node->nid,
        );
        db_insert('tb_theme_feed_contents')->fields($row_contents)->execute();
        $nodes[] = $node;
      }
    }
    return $nodes;
  }

  function createServiceForm($service, &$form) {
    $form[$service->service . "_enable"] = array(
      '#type' => 'checkbox',
      '#title' => t("Enable"),
      '#value' => $service->enabled,
    );

    $settings_form = array(
      '#type' => 'container',
      '#states' => array(
        'visible' => array(
          "input[name='{$service->service}_enable']" => array(
            'checked' => TRUE,
          ),
        ),
      ),
    );

    $this->createAppForm($service, $settings_form);

    $settings_form[$service->service . "_auto_feed"] = array(
      '#type' => 'checkbox',
      '#title' => t("Auto feed"),
      '#value' => $service->auto_feed,
      '#description' => 'Automatically feed items every configured time of the drupal cron. Click <a href="' . base_path() . '/admin/config/system/cron">here</a> to configure the cron ',
    );
    $this->createFeedLimitForm($service, $settings_form);

    $form[$service->service . '_settings_wrapper'] = $settings_form;
  }

  function createFeedLimitForm($service, &$form) {
    $form[$service->service . "_feed_limit"] = array(
      '#type' => 'textfield',
      '#title' => t("Feed Limit"),
      '#value' => $service->feed_limit,
      '#description' => t('Please enter a valid number, the number of feed items returned for each feed request.'),
    );
  }

  function createAppForm($service, &$form) {
    
  }

  function convertLink($domain, $url, $link, $remove_param = FALSE) {
    if ($remove_param) {
      $parts = explode('?', $link);
      $parts = explode('#', $parts[0]);
      $link = $parts[0];
    }
    if ($this->hasProtocol($link)) {
      return $link;
    }
    elseif (($link == '#') || ($link == "/")) {
      return $url;
    }
    elseif (substr($link, 0, 1) == "/") {
      return $domain . $link;
    }
    else {
      return $domain . "/" . $link;
    }
  }

  function hasProtocol($url) {
    return strpos($url, "//");
  }

  public function rssToArray($tags, $array, $url) {

    $doc = new DOMdocument();
    @$doc->load($url);
    $rss_array = array();
    foreach ($tags as $tag) {
      if ($doc->getElementsByTagName($tag)) {
        foreach ($doc->getElementsByTagName($tag) AS $node) {
          $items = array();
          foreach ($array AS $key => $values) {
            $items[$key] = array();
            foreach ($values as $value) {
              if ($itemsCheck = $node->getElementsByTagName($value)) {
                for ($j = 0; $j < $itemsCheck->length; $j++) {
                  if (($attribute = $itemsCheck->item($j)->nodeValue) != "") {
                    $items[$key][] = $attribute;
                  }
                  elseif ($attribute = $itemsCheck->item($j)->getAttribute('term')) {
                    $items[$key][] = $attribute;
                  }
                  elseif ($itemsCheck->item($j)->getAttribute('rel') == 'alternate') {
                    $items[$key][] = $itemsCheck->item($j)->getAttribute('href');
                  }
                }
              }
            }
          }
          array_push($rss_array, $items);
        }
      }
    }
    return $rss_array;
  }

  /**
   * Return feed limit based on number of feeds & user-defined feed limit
   * @param $items array list of items
   * @return $limit number 
   */
  public function getFeedLimit($items) {
    $length = count($items);
    $limit = $this->configs['limit'];
    if ($limit == 0 || $limit > $length)
      $limit = $length;
    return $limit;
  }

  function createAccountForm($account, &$form) {
    $form = NULL;
  }

  public function getBundle() {
    return 'tb_theme_feed';
  }

}
