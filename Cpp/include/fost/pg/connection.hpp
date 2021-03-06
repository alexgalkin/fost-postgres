/**
    Copyright 2015-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <fost/core>


namespace fostlib {


    namespace pg {


        extern const module c_fost_pg;


        class recordset;
        class unbound_procedure;


        /// A read/write database connection. Also provides a low level API
        /// for interacting with the database.
        class connection {
            friend class recordset;
            friend class unbound_procedure;
            struct impl;
            std::unique_ptr<impl> pimpl;

          public:
            /// A default connection without host or password
            connection();
            /// Connect to a specified host without specifying a password
            connection(const string &);
            /// Connect to a specified host and database
            connection(const string &, const string &);
            /// Connect using the provided JSON configuration
            /// Supported items are:
            /// 1. dbname -- Name of the database
            /// 2. host -- The host (or path when starting with /)
            /// 3. password -- Connection password to use
            /// 4. user -- The username
            connection(const json &);

            /// Move constructor
            connection(connection &&cnx);

            /// Destructor so we can link
            ~connection();

            /// Retrieve the connection configuration details
            const json &configuration() const;

            /// Commit the transaction
            void commit();

            /// Configuration options
            connection &zoneinfo(const fostlib::string &zi);
            /// Set a setting for this session
            connection &set_session(
                    const fostlib::string &s, const fostlib::string &v);

            /// Return a recordset range from the execution of the command
            recordset exec(const utf8_string &);
            /// Select statement intended for fetching individual row, or
            /// collections
            recordset select(const char *relation, const json &keys);
            recordset select(
                    const char *relation, const json &keys, const json &order);
            /// Perform a one row INSERT statement. Pass a JSON object that
            /// specifies the field names and values
            connection &insert(const char *relation, const json &values);
            /// Insert using a RETURNING clause so we return a recordset
            recordset
                    insert(const char *relation,
                           const json &values,
                           const std::vector<fostlib::string> &returning);
            /// Perform a one row UPDATE statement. Give the keys and values
            connection &update(
                    const char *relation, const json &keys, const json &values);
            recordset
                    update(const char *relation,
                           const json &keys,
                           const json &values,
                           const std::vector<fostlib::string> &returning);
            /// Perform an UPSERT (INSERT/CONFLICT). Give the keys and values
            /// and optionally a returning list
            connection &upsert(
                    const char *relation, const json &keys, const json &values);
            recordset
                    upsert(const char *relation,
                           const json &keys,
                           const json &values,
                           const std::vector<fostlib::string> &returning);

            /// Create an anonymous stored procedure
            unbound_procedure procedure(const utf8_string &);
        };


        /// Create a database
        void createdb(const json &dsn, const string &dbname);

        /// Drop a database
        void dropdb(const json &dsn, const string &dbname);


    }


}
