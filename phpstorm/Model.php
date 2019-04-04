<?php

namespace Xan\Db;

use Xan\Db\Adapter;

/**
 * Class Model
 *
 * A model was a Db table object to operate the Db database table
 * when using the complex SQL, recommand the Adapter raw SQL mode to do the job.
 *
 * Xan extension contain this component
 * @author Josin<774542602@qq.com>
 * @package Xan
 */
abstract class Model extends Adapter
{
    /**
     * Model constructor.
     *
     * Construct the Model object
     *
     * <pre>
     *      new Adapter('mysql:host=localhost;dbname=xxx;port=3306")
     * </pre>
     *
     * @param $dsn              The connection dsn
     * @param $username         The username to log in Db engine.
     * @param $password         The password with the username above
     * @param array $options    The other options to init the Db adapter
     */
    final function __construct($dsn, $username, $password, $options = []) {
    
    }
    
    /**
     * Return the table name
     * @return string   The full table name which the model to operate
     */
    abstract function tableName();
    
    /**
     * Set the adapter with the adapter object
     * @param string $adapterKey       The adapter key owned by global IOC di container
     * @return mixed
     */
    final function setAdapter($adapterKey) {
    
    }
    
    /**
     * Derived class can override this method to do the init job
     */
    function init() {
    
    }
    
    /**
     * Set the fields of the SQL SELECT fields
     * $fields support array & string format, when string format, the fields will be raw, for example:
     *
     * fields("id, age, name") => SELECT id, age, name
     *
     * but when in array mode:
     *
     * fields(["id", "age", "name"]) => SELECT `id`, `age`, `name`
     * and array support alias:
     * fields(['id' => 'aid']) => SELECT `id` AS aid
     *
     * @param array|string $fields
     * @return Model
     */
    final function fields($fields) {
        return $this;
    }
    
    /**
     * Set the where condition of the SQL SELECT.
     * such as :
     *
     * where(["id" => "2", "price" => ['>=" => '100', '<' => '200]])
     * =>
     * WHERE `id` = 2 AND ( `price` >= 100 AND `price` <= 200 )
     *
     * or string:
     * where("id = 2")
     * =>
     * WHERE id =2
     *
     * @param array|string $where
     * @return Model
     */
    final function where($where) {
        return $this;
    }
    
    /**
     * GROUP BY
     *
     * groupBy('id')
     *
     * @param array|string $groupBy
     * @return Model
     */
    final function groupBy($groupBy) {
        return $this;
    }
    
    /**
     * HAVING
     *
     * same use with the where function
     *
     * @param array|string $having
     * @return Model
     */
    final function having($having) {
        return $this;
    }
    
    /**
     * ORDER BY
     *
     * such as:
     *
     * orderBy(['id' => 'ASC', 'age' => 'DESC'])
     *
     * or
     *
     * orderBy("id ASC, age DESC")
     *
     * @param array|string $orderBy
     * @return Model
     */
    final function orderBy($orderBy) {
        return $this;
    }
    
    /**
     * ActiveRecord feature use __set
     * @param $name
     * @param $value
     */
    final function __set($name, $value) {
    
    }
    
    /**
     * ActiveRecord feature use __get
     * @param $name
     */
    final function __get($name) {
    
    }
    
    /**
     * Get the previous executed SQL
     * @return string
     */
    final function getExecutedSql() {
    
    }
    
    /**
     * LIMIT condition
     *
     * @param int $limit
     * @param int $offset
     * @return Model
     */
    final function limit($limit, $offset) {
        return $this;
    }
    
    /**
     * To return the value as array not the default object
     *
     * by default it return the object not array
     * if you use the asArray calling before one|all
     * it return the array
     * @return Model
     */
    final function asArray() {
        return $this;
    }
    
    /**
     * It support to use the bind value in one() calling to bind the value to the prepared SQL statment
     *
     * @param array $bindValues
     * @return array|void
     */
    function one($bindValues = []) {
    
    }
    
    /**
     * Fetch all result from the Db engine. with the given binding values
     * @param array $bindValues
     * @return array|void
     */
    function all($bindValues = []) {
    
    }
    
    /**
     * Update the table with the given data
     *
     * @param array         $data
     * @param array         $bindValues
     * @param array|string  $where
     * @return bool <b>TRUE</b> on success or <b>FALSE</b> on failure.
     */
    function update($data = [], $bindValues = [], $where) {
    
    }
    
    /**
     * INSERT INTO the DB engine.
     * @param array $data
     * @param array $bindValues
     * @return bool <b>TRUE</b> on success or <b>FALSE</b> on failure.
     */
    function save($data = [], $bindValues = []) {
    
    }
    
    /**
     * DELETE command
     * @param array|string  $where
     * @param array         $bindValues
     * @return bool <b>TRUE</b> on success or <b>FALSE</b> on failure.
     */
    function delete($where, $bindValues = []) {
    
    }
    
    /**
     * Return the affected rows
     * @return int
     */
    function getAffectedRows() {
        return 1;
    }
}
