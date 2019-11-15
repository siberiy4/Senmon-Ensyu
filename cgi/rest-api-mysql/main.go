package main

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"

	_ "github.com/go-sql-driver/mysql"
	"github.com/gorilla/mux"
)

type Post struct {
	ID       string `json:"id"`
	Title    string `json:"title"`
	Speaker  string `json:"name"`
	Content  string `json:"text"`
	Mail     string `json:"mail"`
	PostTime string `json:"time"`
	DBTime string `json:"DBtime"`
}

var db *sql.DB
var err error

func main() {
	db, err = sql.Open("mysql", "root:password@/bbs")
	if err != nil {
		panic(err.Error())
	}
	defer db.Close()

	err = db.Ping()
	if err != nil {
		panic(err.Error())
	}

	router := mux.NewRouter()
	router.HandleFunc("/posts", getPosts).Methods("GET")
	router.HandleFunc("/posts", createPost).Methods("POST")

	http.ListenAndServe(":8080", router)
}

func getPosts(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	var posts []Post
	result, err := db.Query("SELECT * from article")
	if err != nil {
		panic(err.Error())
	}
	for result.Next() {
		var post Post
		err := result.Scan(&post.ID, &post.Title, &post.Speaker , &post.Mail, &post.Content,  &post.PostTime, &post.DBTime)
		if err != nil {
			panic(err.Error())
		}
		posts = append(posts, post)
	}
	json.NewEncoder(w).Encode(posts)
	defer result.Close()
}

func createPost(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	stmt, err := db.Prepare("INSERT INTO bbs.article (title, speaker_name, mail , content, post_time)  VALUES(?,?,?,?,?)")

	if err != nil {
		panic(err.Error())
	}
	body, err := ioutil.ReadAll(r.Body)
	if err != nil {
		panic(err.Error())
	}
	keyVal := make(map[string]string)
	json.Unmarshal(body, &keyVal)

	title := keyVal["title"]
	speaker_name := keyVal["name"]
	mail := keyVal["mail"]
	content := keyVal["text"]
	post_time := keyVal["time"]

	_, err = stmt.Exec(title,speaker_name,mail,content,post_time);
	if err != nil {
		panic(err.Error())
	}

	fmt.Fprintf(w, "New post was created")
}
