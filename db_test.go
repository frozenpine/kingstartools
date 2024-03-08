package kingstartools

import "testing"

func TestSybaseConn(t *testing.T) {
	if err := connect("sa", "sybase15", "172.16.201.11", 2048, "ksqhdb"); err != nil {
		t.Fatal(err)
	}
}
