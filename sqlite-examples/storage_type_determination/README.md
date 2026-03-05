# Storage Type Determination

SQLite is frequently described as a **typeless database engine**. This means it does not enforce strict domain constraints on table columns.

In most cases, **any type of value can be stored in any column**, regardless of the column's declared SQL type.

There is one notable exception: the **INTEGER PRIMARY KEY column** (the `rowid`). This column can only store integer values. If the VM encounters a value that cannot be interpreted as an integer for this column, the insertion is rejected.

SQLite even allows tables to be created **without specifying column types at all**:

```sql
CREATE TABLE T1(a, b, c);
```

Since there is no strict typing requirement, the question becomes:
**How does SQLite decide what storage type a value should have?**

The VM determines the initial storage type based on how the value enters the system.


## 1. Values Specified as SQL Literals

When a value appears directly in an SQL statement, SQLite determines its storage type according to its syntax.

Examples:

* **TEXT**
  Values enclosed in quotes are interpreted as strings.

```sql
INSERT INTO t1 VALUES('hello');
```

* **INTEGER**
  Unquoted numbers without a decimal point or exponent.

```sql
INSERT INTO t1 VALUES(123);
```

* **REAL**
  Numbers containing a decimal point or exponent.

```sql
INSERT INTO t1 VALUES(3.14);
INSERT INTO t1 VALUES(2e5);
```

* **NULL**

```sql
INSERT INTO t1 VALUES(NULL);
```

* **BLOB**

```sql
INSERT INTO t1 VALUES(X'ABCD');
```

In this notation, the hexadecimal digits define the raw byte sequence stored in the database.

If a value does not match any of these patterns, the VM rejects it and query execution fails.
