<?php

class TBTwitterFeed extends TBSocialFeed {

  function __construct($cfg = NULL) {
    $this->configs = array(
      'account' => $cfg ? $cfg['account'] : 'themebrain',
      'limit' => ($cfg['feed_limit'] > 100) ? 100 : $cfg['feed_limit']
    );
  }

  function getType() {
    return "Twitter";
  }

  public function getContent() {
    $endpoint = 'http://search.twitter.com/search.json?q=%s&rpp=%s';
    $fullurl = sprintf($endpoint, $this->configs['account'], $this->configs['limit']);
    $raw = file_get_contents($fullurl);
    $data = json_decode($raw);
    if (!isset($data->results)) {
      return FALSE;
    }
    $results = array();
    for ($i = 0; $i < count($data->results); $i++) {
      $row = $data->results[$i];
      $result = array();
      $result['title'] = $row->from_user_name;
      $result['created'] = $result['changed'] = strtotime($row->created_at);
      $result['body'] = $row->text;
      $result['field_social_feed_type'] = $this->getType();
      $result['node_type'] = $this->getBundle();
      $result['feed_id'] = $row->id_str;
      $result['url'] = $fullurl;
      $result['field_reference_url'] = $result['reference_url'] = 'https://twitter.com/' . $this->configs['account'] . '/statuses/' . $row->id_str;
      $result['service'] = 'twitter';
      $result['account'] = $this->configs['account'];
      $results[] = $result;
    }
    return $results;
  }

  function createAccountForm($account, &$form) {
    $form = array(
      '#type' => 'fieldset',
      '#title' => t("Account")
    );
    $form[$account->service . '_feed_account_name[]'] = array(
      '#type' => 'textfield',
      '#size' => 8,
      '#title' => t("Account Name"),
      '#default_value' => isset($account->account_name) ? $account->account_name : ""
    );
    $form[$account->service . '_feed_account[]'] = array(
      '#type' => 'textfield',
      '#size' => 8,
      '#title' => t("Search Keyword"),
      '#default_value' => isset($account->account) ? $account->account : "",
      '#description' => t('Enter your search keyword. Ex: mashable, from:joomlart, to:9gag, etc. Click <a href="https://dev.twitter.com/docs/using-search">here</a> to see other options for search keyword'),
      '#attributes' => array('class' => array('account')),
    );
    $form[$account->service . '_feed_current_account[]'] = array(
      '#type' => 'hidden',
      '#value' => isset($account->account) ? $account->account : ""
    );
    $form[$account->service . '_feed_account_verify'] = array(
      '#markup' => '<a href="javascript:void(0)" class="form-submit btn-verify-account" onclick="Drupal.TBFeed.verifyAccount(\'http://search.twitter.com/search.rss?q=\', this)">' . t('Check feed') . '</a><br/>',
    );
    $form[$account->service . '_feed_account_delete'] = array(
      '#markup' => '<a href="javascript:void(0)" class="btn btn-delete-account" onclick="Drupal.TBFeed.deleteAccount(this)">' . t('Delete') . '</a>',
    );
  }

  function createFeedLimitForm($service, &$form) {
    $form[$service->service . "_feed_limit"] = array(
      '#type' => 'textfield',
      '#title' => t("Feed Limit"),
      '#value' => $service->feed_limit,
      '#description' => t('The valid number of feed items for Twitter should not be higher than 100.'),
    );
  }

}