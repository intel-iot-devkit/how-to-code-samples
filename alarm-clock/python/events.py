from event_emitter import EventEmitter, on
from apscheduler.schedulers.background import BackgroundScheduler

emitter = EventEmitter()
scheduler = BackgroundScheduler()

def ms(ms):

    """
    Converts milliseconds to seconds
    """

    return ms * 0.001

scheduler.start()