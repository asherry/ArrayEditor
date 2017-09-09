<?php

/**
 * @file
 * ArrayEditor class.
 */

/**
 * Add values to an array.
 *
 * Example:
 * @code
 * $array = array(
 *   'zero' => '0',
 *   'one'  => '1',
 *   'two'  => '2',
 *   'three'=> '3',
 * );
 *
 * $array = array(15, 20, 25, 30, 35, 40);
 *
 * $test1 = ArrayEditor::create($array, TRUE)->key(2)->append('value');
 * $test2 = ArrayEditor::create($array, TRUE)->value(35)->prepend('value', 'foo');
 *
 * $arr = ArrayEditor::create($array);
 * $arr->value(15)->append('16');
 * $arr->key(4)->append(37);
 * $test3 = $arr->value(35)->prepend(34);
 * @endcode
 */
// @codingStandardsIgnoreStart
class ArrayEditor {
// @codingStandardsIgnoreEnd

  /**
   * Core array.
   *
   * @var array
   */
  private $array;

  /**
   * Current key used in chain.
   *
   * @var mixed
   */
  private $key;

  /**
   * Current method to search for a particular index.
   *
   * @var string
   */
  private $searchMethod;

  /**
   * Current value we are searching by.
   *
   * @var string
   */
  private $searchValue;

  /**
   * Where we are placing the new value.
   *
   * @var string
   */
  private $placement;

  /**
   * Show drupal_set_message.
   *
   * @var bool
   */
  private $debug;

  /**
   * PHP Constructor.
   */
  public function __construct(array $array, $debug = FALSE) {
    $this->debug = $debug;
    $this->array = $array;
  }

  /**
   * Create a new instance.
   */
  public static function create($array, $debug = FALSE) {
    return new ArrayEditor($array, $debug);
  }

  /**
   * Fetch array item by key.
   *
   * @return ArrayEditor
   *   Current object to use in chain methods.
   */
  public function key($key) {
    $this->key = $key;
    $this->searchValue = $key;
    $this->searchMethod = 'key';
    return $this;
  }

  /**
   * Fetch array item by value.
   *
   * @return ArrayEditor
   *   Current object to use in chain methods.
   */
  public function value($value) {
    $this->key = array_search($value, $this->array);
    $this->searchValue = $value;
    $this->searchMethod = 'value';
    return $this;
  }

  /**
   * Insert value after current item.
   *
   * @return array
   *   Current array value.
   */
  public function after($value, $key = NULL) {
    $this->key++;
    $this->placement = 'after';
    return $this->insert($value, $key);
  }

  /**
   * Alias for ArrayEditor::before().
   */
  public function append($value, $key = NULL) {
    return $this->after($value, $key);
  }

  /**
   * Insert value before current item.
   *
   * @return array
   *   Current array value.
   */
  public function before($value, $key = NULL) {
    $this->placement = 'before';
    return $this->insert($value, $key);
  }

  /**
   * Alias for ArrayEditor::before().
   */
  public function prepend($value, $key = NULL) {
    return $this->before($value, $key);
  }

  /**
   * Internal method to add the value in the proper place.
   */
  protected function insert($value, $key) {
    if (isset($this->array[$this->key])) {
      $newArray = !is_null($key) ? array($key => $value) : array($value);
      $slice1 = array_slice($this->array, 0, $this->key, TRUE);
      $slice2 = array_slice($this->array, $this->key, count($this->array) - 1, TRUE);
      $this->array = array_merge($slice1, $newArray, $slice2);
    }
    $this->debug($value, $key);
    $this->clear();
    return $this->array;
  }

  /**
   * Show debug info with drupal_set_message.
   */
  protected function debug($value, $key) {
    if (!$this->debug) {
      return;
    }
    drupal_set_message(t('Inserted value "@value" @placement @searchMethod @searchValue@newKey.', array(
      '@value' => $value,
      '@placement' => $this->placement,
      '@searchMethod' => $this->searchMethod,
      '@searchValue' => $this->searchValue,
      '@newKey' => $key ? ' with new key ' . $key : '',
    )));
  }

  /**
   * Clear temp data from chain methods.
   */
  protected function clear() {
    unset($this->placement);
    unset($this->searchMethod);
    unset($this->searchKey);
    unset($this->key);
  }

}
