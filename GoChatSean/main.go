package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"strconv"
	"strings"

	"github.com/gorilla/websocket"
	"github.com/joho/godotenv"
	"golang.org/x/term"
)

func getUser(serverURL string, username string) int {

	endpointURL := serverURL + "/account"
	fmt.Println(endpointURL)
	req, err := http.NewRequest(http.MethodGet, endpointURL, strings.NewReader(username))
	if err != nil {
		log.Fatal(err)
	}

	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println("Error fr fr")
	}
	defer resp.Body.Close()
	body, err := io.ReadAll(resp.Body)
	fmt.Println(string(body))
	userId, err := strconv.Atoi(strings.TrimSpace(string(body)))
	if err != nil {
		log.Fatal(err)
	}
	return userId
}

func loginUser(serverURL string, payload string) uint64 {
	fmt.Println(payload)
	endpointURL := serverURL + "/login"
	req, err := http.NewRequest(http.MethodPost, endpointURL, strings.NewReader(payload))
	if err != nil {
		log.Fatal(err)
	}

	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println("Error fr fr")
	}
	defer resp.Body.Close()
	body, err := io.ReadAll(resp.Body)
	fmt.Println(string(body))
	token, err := strconv.ParseUint(strings.TrimSpace(string(body)), 10, 64)
	if err != nil {
		log.Fatal(err)
	}
	return token
}

func getMessages(serverURL string) {
	wsURL := strings.Replace(serverURL, "http://", "ws://", 1)
	wsURL = strings.Replace(wsURL, "https://", "wss://", 1)
	endpointURL := wsURL + "/ws"

	conn, _, err := websocket.DefaultDialer.Dial(endpointURL, nil)
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()

	_, message, err := conn.ReadMessage()
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(string(message))
}

func main() {
	username := flag.String("username", "Guest", "Your username")
	flag.Parse()

	if err := godotenv.Load(); err != nil {
		log.Println("No .env file found, relying on real environment variables")
	}

	serverURL := os.Getenv("SERVER_URL")
	fmt.Println("Server: ", serverURL)

	userId := getUser(serverURL, *username)
	if userId == 0 {
		// Create account
		fmt.Printf("No account found for %s. Please text Sean to setup your account.\n", *username)
	} else {
		// Login
		fmt.Printf("Please enter the password for %s: ", *username)
		password, err := term.ReadPassword(int(os.Stdin.Fd()))
		if err != nil {
			log.Fatal(err)
		}
		fmt.Println()
		fmt.Println("Password received, length:", len(password))
		// Send "username password" to /login
		payload := *username + " " + string(password)
		token := loginUser(serverURL, payload)
		if token == 0 {
			// Unsuccessful login
		} else {
			// Successful login!
			getMessages(serverURL)
		}
	}
}
