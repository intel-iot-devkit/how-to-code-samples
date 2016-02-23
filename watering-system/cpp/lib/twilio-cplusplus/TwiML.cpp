/*
 * Twilio REST C++
 * Author: Laurent Luce
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

#include "Utils.h"
#include "Rest.h"
#include "TwiML.h"

using namespace twilio;

/**
  * Append a children verb
  * @param verb Twilio verb
  * @return verb
*/
Verb& Verb::append(Verb& verb)
{
  vector<string>::iterator result;
  result = find(allowedVerbs.begin(), allowedVerbs.end(), verb.getTag());
  if(result == allowedVerbs.end())
  {
    throw "This is not a supported verb";
  }
  else
  {
    children.push_back(verb);
    return verb;
  }
}

/**
  * Get verb body
  * @return verb body
*/
string& Verb::getBody()
{
  return tbody;
}

/**
  * Get verb tag
  * @return verb tag
*/
string& Verb::getTag()
{
  return ttag;
}

/**
  * Get verb children
  * @return vector of verbs
*/
vector<Verb>& Verb::getChildren()
{
  return children;
}

/**
  * Get verb attributes
  * @return vector of attributes
*/
vector<Var>& Verb::getAttributes()
{
  return attributes;
}

/**
  * Return verb xml based on tag, body, attributes and its children
  * @return verb xml
*/
string Verb::toXML()
{
  string xml = "<" + ttag;
  for (unsigned int i = 0; i < attributes.size(); i++)
  {
    xml += " " + attributes[i].key + "=\"" + attributes[i].value + "\"";
  }
  
  xml += ">";
  
  if(tbody.length() > 0)
    xml += "<![CDATA[" + replaceAll(tbody, "]]>", "]]>]]><![CDATA[") + "]]>";
  for (unsigned int i = 0; i < children.size(); i++)
  {
    xml += children[i].toXML();
  }
  
  return xml += "</" + ttag + ">";
}

/**
 * Set attribute value. Add attribute if doesn't exist
 * @param key attribute key
 * @param value attribute value
*/
void Verb::set(const string& key, const string& value)
{
  for(unsigned int i = 0; i < attributes.size(); i++)
  {
    if(attributes[i].key == key)
    {
      attributes[i].value = value;
      return;
    }
  }

  attributes.push_back(Var(key, value));
}

/**
 * Set loop attribute
 * @param i loop count
*/
void Say::setLoop(int i)
{
  set("loop", intToStr(i));
}

/**
 * Set language attribute
 * @param str language
*/
void Say::setLanguage(const string& str)
{
  set("language", str);
}

/**
 * Set voice attribute
 * @param str language
*/
void Say::setVoice(const string& str)
{
  set("voice", str);
}

/**
 * Set muted attribute
 * @param b muted attribute value
*/
void Conference::setBoolean(const string& attr, bool b)
{
  if(b)
    set(attr, "true");
  else
    set(attr, "false");
}

/**
 * Set muted attribute
 * @param b muted attribute value
*/
void Conference::setMuted(bool b)
{
  setBoolean("muted", b);
}

/**
 * Set beep attribute
 * @param b beep atttribute value
*/
void Conference::setBeep(bool b)
{
  setBoolean("beep", b);
}

/**
 * Set startConferenceOnEnter attribute
 * @param b startConferenceOnEnter attribute value
*/
void Conference::setStartConferenceOnEnter(bool b)
{
  setBoolean("startConferenceOnEnter", b);
}

/**
 * Set endConferenceOnExit attribute
 * @param b endConferenceOnExit attribute value
*/
void Conference::setEndConferenceOnExit(bool b)
{
  setBoolean("endConferenceOnExit", b);
}

/**
 * Set waitMethod
 * @param s waitMethod attribute value
*/
void Conference::setWaitMethod(const string& s)
{
  set("waitMethod", s);
}

/**
 * Set waitUrl
 * @param s waitUrl attribute value
*/
void Conference::setWaitUrl(const string& s)
{
  set("waitUrl", s);
}
    
/**
  * Set action attribute 
  * @param s action attribute value
*/
void Dial::setAction(const string& s)
{
  set("action", s);
}

/**
  * Set method attribute
  * @param s attribute value
*/
void Dial::setMethod(const string& s)
{
  set("method", s);
}

/**
  * Set caller id attribute
  * @param s attribute value
*/
void Dial::setCallerId(const string& s)
{
  set("callerId", s);
}

/**
 * Set timeout attribute
 * @param i value
*/
void Dial::setTimeout(int i)
{
  set("timeout", intToStr(i));
}

/**
 * Set timeLimit attribute
 * @param i value
*/
void Dial::setTimeLimit(int i)
{
  set("timeLimit", intToStr(i));
}

/**
 * Set hangupOnStar attribute
 * @param b value
*/
void Dial::setHangupOnStar(bool b)
{
  if(b)
    set("hangupOnStar", "true");
  else
    set("hangupOnStar", "false");
}

/**
  * Set action attribute 
  * @param s action attribute value
*/
void Gather::setAction(const string& s)
{
  set("action", s);
}

/**
  * Set method attribute
  * @param s attribute value
*/
void Gather::setMethod(const string& s)
{
  set("method", s);
}

/**
 * Set timeout attribute
 * @param i value
*/
void Gather::setTimeout(int i)
{
  set("timeout", intToStr(i));
}

/**
 * Set numDigits attribute
 * @param i value
*/
void Gather::setNumDigits(int i)
{
  set("numDigits", intToStr(i));
}

/**
  * Set finishOnKey attribute
  * @param s attribute value
*/
void Gather::setFinishOnKey(const string& s)
{
  set("finishOnKey", s);
}

/**
 * Set sendDigits attribute
 * @param s attribute value
*/
void Number::setSendDigits(const string& s)
{
  set("sendDigits", s);
}

/**
 * Set url attribute
 * @param s attribute value
*/
void Number::setUrl(const string& s)
{
  set("url", s);
}

/**
 * Set lenght attribute
 * @param i value
*/
void Pause::setLength(int i)
{
  set("length", intToStr(i));
}

/**
 * Set loop attribute
 * @param i loop count
*/
void Play::setLoop(int i)
{
  set("loop", intToStr(i));
}

/**
  * Set action attribute 
  * @param s action attribute value
*/
void Record::setAction(const string& s)
{
  set("action", s);
}

/**
  * Set method attribute
  * @param s attribute value
*/
void Record::setMethod(const string& s)
{
  set("method", s);
}

/**
 * Set timeout attribute
 * @param i value
*/
void Record::setTimeout(int i)
{
  set("timeout", intToStr(i));
}

/**
  * Set finishOnKey attribute
  * @param s attribute value
*/
void Record::setFinishOnKey(const string& s)
{
  set("finishOnKey", s);
}

/**
 * Set maxLength attribute
 * @param i value
*/
void Record::setMaxLength(int i)
{
  set("maxLength", intToStr(i));
}

/**
  * Set transcribeCallback attribute
  * @param s attribute value
*/
void Record::setTranscribeCallback(const string& s)
{
  set("transcribeCallback", s);
}

/**
 * Set transcribe attribute
 * @param b value
*/
void Record::setTranscribe(bool b)
{
  if(b)
    set("transcribe", "true");
  else
    set("transcribe", "false");
}

/**
  * Set method attribute
  * @param s attribute value
*/
void Redirect::setMethod(const string& s)
{
  set("method", s);
}

/**
  * Set reason attribute
  * @param s attribute value
*/
void Reject::setReason(const string& s)
{
  set("reason", s);
}

/**
  * Set action attribute 
  * @param s action attribute value
*/
void Sms::setAction(const string& s)
{
  set("action", s);
}

/**
  * Set method attribute
  * @param s attribute value
*/
void Sms::setMethod(const string& s)
{
  set("method", s);
}

/**
  * Set to attribute
  * @param s attribute value
*/
void Sms::setTo(const string& s)
{
  set("to", s);
}

/**
  * Set from attribute
  * @param s attribute value
*/
void Sms::setFrom(const string& s)
{
  set("from", s);
}

/**
  * Set statusCallback attribute
  * @param s attribute value
*/
void Sms::setStatusCallback(const string& s)
{
  set("statusCallback", s);
}

const string Verb::V_SAY = "Say";
const string Verb::V_PLAY = "Play";
const string Verb::V_GATHER = "Gather";
const string Verb::V_RECORD = "Record";
const string Verb::V_PAUSE = "Pause";
const string Verb::V_HANGUP = "Hangup";
const string Verb::V_DIAL = "Dial";
const string Verb::V_NUMBER = "Number";
const string Verb::V_REDIRECT = "Redirect";
const string Verb::V_RESPONSE = "Response";
const string Verb::V_CONFERENCE = "Conference";
const string Verb::V_SMS = "Sms";
const string Verb::V_REJECT = "Reject";


