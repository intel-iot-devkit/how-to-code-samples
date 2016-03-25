// Golang command line utility to generate an Microsoft Azure SAS token for use
// with the MS Azure IoT Hub.
//
// USAGE: USAGE: sastoken <URI> <key> <expiresInMins> [policy]
//
// Copyright © 2016 The Hybrid Group (http://hybridgroup.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/base64"
	"fmt"
	"log"
	"os"
	"strconv"
	"text/template"
	"time"
)

func ComputeHmac256(message string, secret string) string {
	data, _ := base64.StdEncoding.DecodeString(secret)
	key := []byte(data)
	h := hmac.New(sha256.New, key)
	h.Write([]byte(message))
	return base64.StdEncoding.EncodeToString(h.Sum(nil))
}

func GenerateSasToken(resourceUri string, signingKey string, expiresInMins string, policyName string) string {
	uri := template.URLQueryEscaper(resourceUri)

	duration, _ := strconv.Atoi(expiresInMins)
	expire := time.Now().Add(time.Duration(duration) * time.Minute)
	secs := expire.Unix()
	signed := uri + "\n" + strconv.FormatInt(secs, 10)

	val := ComputeHmac256(signed, signingKey)
	encoded_val := template.URLQueryEscaper(val)

	token := "SharedAccessSignature sr=" + uri + "&sig=" + encoded_val + "&se=" + strconv.FormatInt(secs, 10)
	if len(policyName) > 0 {
		token += "&skn=" + policyName
	}

	return token
}

func main() {
	if len(os.Args) < 4 {
		log.Fatal("USAGE: sastoken <URI> <key> <expiresInMins> [policy]")
	}

	policy := ""
	if len(os.Args) == 5 {
		policy = os.Args[4]
	}

	token := GenerateSasToken(os.Args[1], os.Args[2], os.Args[3], policy)
	fmt.Println(token)
}
