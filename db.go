package kingstartools

import (
	"database/sql"
	"errors"
	"strconv"

	"github.com/SAP/go-ase"
)

var db *sql.DB

func connect(user, pass string, host string, port int, name string) (err error) {
	info, err := ase.NewInfo()
	if err != nil {
		return err
	}

	info.Host = host
	info.Port = strconv.Itoa(port)
	info.Username = user
	info.Password = pass
	info.DebugLogPackages = true
	info.Database = name
	info.Network = "tcp"

	conn, err := ase.NewConnector(info)
	if err != nil {
		return err
	}

	db = sql.OpenDB(conn)
	if db == nil {
		return errors.New("open db failed")
	}

	if err = db.Ping(); err != nil {
		return
	}

	return
}
