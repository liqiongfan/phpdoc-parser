<?php

namespace Xan\Db;

/**
 * Class Adapter
 *
 * Adapter was an wrapper PDO object to do the DB operation job.
 *
 * Xan extension contains the component
 *
 * @author Josin<774542602@qq.com>
 * @package Xan\Db
 */
class Adapter
{
    /**
     * Adapter constructor.
     *
     * To construct an adapter with the given DB engine DSN, such as MySQL:
     *
     * <pre>
     *      new Adapter('mysql:host=localhost;dbname=xxx;port=3306")
     * </pre>
     *
     * @param string $dsn          The connection dsn
     * @param string $username     The username to log in Db engine.
     * @param string $password     The password with the username above
     * @param array $options       The other options to init the Db adapter
     */
    function __construct($dsn, $username, $password, $options = []) {
    }
    
    /**
     * To create a prepared SQL, can use the ? or :x to replace the real value
     * and the real value to bind with the execute step. such as:
     *
     * <pre>
     *      $adapter->createQueryCommand("SELECT * FROM xxx WHERE pid = ?");
     *      $data = $adapter->execute([1])->one();
     * </pre>
     *
     * @param string $querySql
     * @param array  $bindValues
     * @return Adapter
     */
    function createQueryCommand($querySql, $bindValues = []) {
        return $this;
    }
    
    /**
     * To execute the prepared SQL with the given bind values if given.
     * most recommend to use the createQueryCommand() & execute & one | all
     * to obtain the real data from Db. to prevent Db attack.
     *
     * @param array $bindValues     The value which to bind to the prepared SQL
     * @return bool <b>TRUE</b> on success or <b>FALSE</b> on failure.
     */
    function execute($bindValues = []) {

    }
    
    /**
     * To do the `INSERT` | `UPATE` | `DELETE` | `CREATE` | `ALTER` SQL and so on...
     * This method return true or false to given the result of the operation.
     * @param string $execSql
     * @return $this
     */
    function execCommand($execSql, $bindValues = []) {
        return $this;
    }
    
    /**
     * To get the raw PDO object binding to the current Adapter object.
     *
     * @return \PDO
     */
    function getPdo() {
        return new \PDO();
    }
    
    /**
     * After doing the createQueryCommand() method invoking job.
     * developers can calling the getPdoStatement() to obtain the PDOStatement Object
     *
     * @return \PDOStatement
     */
    function getPdoStatement() {
        return new \PDOStatement();
    }
    
    /**
     * Return the Query result
     * @return array
     */
    function one() {
        return [];
    }
    
    /**
     * Return the Query result.
     * @return array
     */
    function all() {
        return [];
    }
    
    /**
     * Start the transaction
     */
    function beginTransaction() {
    }
    
    /**
     * Rollback the transaction
     */
    function rollBack() {
    }
    
    /**
     * Commit the curernt transaction
     */
    function commit() {
    }
}