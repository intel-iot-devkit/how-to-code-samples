/*
 * Twilio REST C++
 * Author: Laurent Luce
*/

#ifndef TWILIOTWIML_H
#define TWILIOTWIML_H

namespace twilio {

  class Verb {
    public:
      /**
       * Constructor
       * @param tag TwiML tag
       * @param body TwiML tag body
       */
      Verb(const string& tag, const string& body)
      {
         ttag = tag;
         tbody = body;
      }

      static const string V_SAY;
      static const string V_PLAY;
      static const string V_GATHER;
      static const string V_RECORD;
      static const string V_PAUSE;
      static const string V_HANGUP;
      static const string V_DIAL;
      static const string V_NUMBER;
      static const string V_REDIRECT;
      static const string V_RESPONSE;
      static const string V_CONFERENCE;
      static const string V_SMS;
      static const string V_REJECT;

      /**
       * Append a children verb
       * @param verb Twilio verb
       * @return verb
       */
      Verb& append(Verb& verb);
      
      /**
        * Get verb body
        * @return verb body
      */
      string& getBody();

      /**
        * Get verb tag
        * @return verb tag
      */
      string& getTag();
      
      /**
        * Get verb children
        * @return vector of verbs
      */
      vector<Verb>& getChildren();

      /**
        * Get verb attributes
        * @return vector of attributes
      */
      vector<Var>& getAttributes();
      
      /**
        * Return verb xml based on tag, body, attributes and its children
        * @return verb xml
      */
      string toXML();

      /**
       * Set attribute value. Add attribute if doesn't exist
       * @param key attribute key
       * @param value attribute value
      */
      void set(const string& key, const string& value);

    protected:
      /** tag */
      string ttag;
      /** tag body */
      string tbody;
      /** attributes */
      vector<Var> attributes;
      /** children */
      vector<Verb> children;
      /** allowed verbs */
      vector<string> allowedVerbs;

  };

  class Say: public Verb {
    public:
      /**
       * Constructor
       * @param body tag body
       */
      Say(const string& body) : Verb(V_SAY, body) {};
      
      /**
       * Set loop attribute
       * @param i loop count
      */
      void setLoop(int i);
      
      /**
       * Set language attribute
       * @param str language
      */
      void setLanguage(const string& str);

      /**
       * Set voice attribute
       * @param str language
      */
      void setVoice(const string& str);

  };

  class Conference: public Verb {
    public:
      /**
       * Constructor
       * @param body tag body
       */
      Conference(const string& body) : Verb(V_CONFERENCE, body) {};
    
      /**
       * Set muted attribute
       * @param b muted attribute value
      */
      void setMuted(bool b);

      /**
       * Set beep attribute
       * @param b beep atttribute value
      */
      void setBeep(bool b);

      /**
       * Set startConferenceOnEnter attribute
       * @param b startConferenceOnEnter attribute value
      */
      void setStartConferenceOnEnter(bool b);

      /**
       * Set endConferenceOnExit attribute
       * @param b endConferenceOnExit attribute value
      */
      void setEndConferenceOnExit(bool b);

      /**
       * Set waitMethod
       * @param s waitMethod attribute value
      */
      void setWaitMethod(const string& s);

      /**
       * Set waitUrl
       * @param s waitUrl attribute value
      */
      void setWaitUrl(const string& s);

    private:
      /**
       * Set boolean attribute
       * @param attr attribute name
       * @param b boolean value
      */
      void setBoolean(const string& attr, bool b);
  };

  class Dial: public Verb {
    public:
      /**
       * Constructor
       */
      Dial() : Verb(V_DIAL, "")
      {
        allowedVerbs.push_back(V_NUMBER);
        allowedVerbs.push_back(V_CONFERENCE);
      }
   
      /**
       * Constructor
       * @param number dial number
       */
      Dial(const string& number) : Verb(V_DIAL, number)
      {
        allowedVerbs.push_back(V_NUMBER);
        allowedVerbs.push_back(V_CONFERENCE);
      }
   
      /**
       * Set action attribute 
       * @param s action attribute value
      */
      void setAction(const string& s);

      /**
        * Set method attribute
        * @param s attribute value
      */
      void setMethod(const string& s);

      /**
        * Set caller id attribute
        * @param s attribute value
      */
      void setCallerId(const string& s);

      /**
       * Set timeout attribute
       * @param i value
      */
      void setTimeout(int i);

      /**
       * Set timeLimit attribute
       * @param i value
      */
      void setTimeLimit(int i);

      /**
       * Set hangupOnStar attribute
       * @param i value
      */
      void setHangupOnStar(bool b);

  };

  class Gather: public Verb {
    public:
      /**
       * Constructor
       */
      Gather() : Verb(V_GATHER, "")
      {
        allowedVerbs.push_back(V_SAY);
        allowedVerbs.push_back(V_PLAY);
        allowedVerbs.push_back(V_PAUSE);
      }

      /**
        * Set action attribute 
        * @param s action attribute value
      */
      void setAction(const string& s);

      /**
        * Set method attribute
        * @param s attribute value
      */
      void setMethod(const string& s);

      /**
       * Set timeout attribute
       * @param i value
      */
      void setTimeout(int i);

      /**
       * Set numDigits attribute
       * @param i value
      */
      void setNumDigits(int i);

      /**
        * Set finishOnKey attribute
        * @param s attribute value
      */
      void setFinishOnKey(const string& s);

  };

  class Hangup: public Verb {
    public:
      /**
       * Constructor
       */
      Hangup() : Verb(V_GATHER, "")
      {
      }
  };

  class Number: public Verb {
    public:
      /**
       * Constructor
       * @param number
       */
      Number(const string& number) : Verb(V_NUMBER, number)
      {
      }
      
      /**
       * Set sendDigits attribute
       * @param s attribute value
      */
      void setSendDigits(const string& s);

      /**
       * Set url attribute
       * @param s attribute value
      */
      void setUrl(const string& s);
  };

  class Pause: public Verb {
    public:
      /**
       * Constructor
       */
      Pause() : Verb(V_PAUSE, "")
      {
      }
      
      /**
       * Set lenght attribute
       * @param i value
      */
      void setLength(int i);
  };

  class Play: public Verb {
    public:
      /**
       * Constructor
       */
      Play(const string& body) : Verb(V_PLAY, body)
      {
      }
      
      /**
       * Set loop attribute
       * @param i value
      */
      void setLoop(int i);
  };

  class Record: public Verb {
    public:
      /**
       * Constructor
       */
      Record() : Verb(V_RECORD, "")
      {
      }
     
      /**
       * Set action attribute 
       * @param s action attribute value
      */
      void setAction(const string& s);

      /**
        * Set method attribute
        * @param s attribute value
      */
      void setMethod(const string& s);

      /**
       * Set timeout attribute
       * @param i value
      */
      void setTimeout(int i);

      /**
        * Set finishOnKey attribute
        * @param s attribute value
      */
      void setFinishOnKey(const string& s);

      /**
       * Set maxLength attribute
       * @param i value
      */
      void setMaxLength(int i);

      /**
        * Set transcribeCallback attribute
        * @param s attribute value
      */
      void setTranscribeCallback(const string& s);

      /**
       * Set transcribe attribute
       * @param b value
      */
      void setTranscribe(bool b);
   
  };

  class Redirect: public Verb {
    public:
      /**
       * Constructor
       */
      Redirect() : Verb(V_REDIRECT, "")
      {
      }
   
      /**
       * Constructor
       * @param number redirect number
       */
      Redirect(const string& number) : Verb(V_REDIRECT, number)
      {
      }
   
      /**
        * Set method attribute
        * @param s attribute value
      */
      void setMethod(const string& s);
  };

  class Reject: public Verb {
    public:
      /**
       * Constructor
       */
      Reject() : Verb(V_REJECT, "")
      {
      }
   
      /**
        * Set reason attribute
        * @param s attribute value
      */
      void setReason(const string& s);
  };

  class Sms: public Verb {
    public:
      /**
       * Constructor
       * @param message sms message
       */
      Sms(const string& message) : Verb(V_SMS, message)
      {
      }
   
      /**
       * Set action attribute 
       * @param s action attribute value
      */
      void setAction(const string& s);

      /**
        * Set method attribute
        * @param s attribute value
      */
      void setMethod(const string& s);

      /**
        * Set to attribute
        * @param s attribute value
      */
      void setTo(const string& s);

      /**
        * Set from attribute
        * @param s attribute value
      */
      void setFrom(const string& s);

      /**
        * Set statusCallback attribute
        * @param s attribute value
      */
      void setStatusCallback(const string& s);

  };

  class TwiMLResponse: public Verb {
    public:
      /**
       * Constructor
       */
      TwiMLResponse() : Verb(V_RESPONSE, "")
      {
        allowedVerbs.push_back(V_GATHER);
        allowedVerbs.push_back(V_RECORD);
        allowedVerbs.push_back(V_DIAL);
        allowedVerbs.push_back(V_SAY);
        allowedVerbs.push_back(V_PLAY);
        allowedVerbs.push_back(V_REDIRECT);
        allowedVerbs.push_back(V_REJECT);
        allowedVerbs.push_back(V_HANGUP);
        allowedVerbs.push_back(V_PAUSE);
        allowedVerbs.push_back(V_SMS);
      }
  };
}

#endif
