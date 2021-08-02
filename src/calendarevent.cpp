/*
 * Copyright (c) 2013 - 2019 Jolla Ltd.
 * Copyright (c) 2020 - 2021 Open Mobile Platform LLC.
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "calendarevent.h"

#include <QQmlInfo>
#include <QDateTime>
#include <QTimeZone>

#include "calendarutils.h"
#include "calendarmanager.h"

CalendarEvent::CalendarEvent(CalendarManager *manager, const QString &uid, const QDateTime &recurrenceId)
    : QObject(manager), mManager(manager), mUniqueId(uid), mRecurrenceId(recurrenceId)
{
    connect(mManager, SIGNAL(notebookColorChanged(QString)),
            this, SLOT(notebookColorChanged(QString)));
    connect(mManager, SIGNAL(eventUidChanged(QString,QString)),
            this, SLOT(eventUidChanged(QString,QString)));
}

CalendarEvent::~CalendarEvent()
{
}

QString CalendarEvent::displayLabel() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).displayLabel;
}

QString CalendarEvent::description() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).description;
}

QDateTime CalendarEvent::startTime() const
{
    // Cannot return the date time directly here. If UTC, the QDateTime
    // will be in UTC also and the UI will convert it to local when displaying
    // the time, while in every other case, it set the QDateTime in
    // local zone.
    const QDateTime dt = mManager->getEvent(mUniqueId, mRecurrenceId).startTime;
    return QDateTime(dt.date(), dt.time());
}

QDateTime CalendarEvent::endTime() const
{
    const QDateTime dt = mManager->getEvent(mUniqueId, mRecurrenceId).endTime;
    return QDateTime(dt.date(), dt.time());
}

static Qt::TimeSpec toTimeSpec(const QDateTime &dt)
{
    if (dt.timeZone() == QTimeZone::utc()) {
        return Qt::UTC;
    }

    return dt.timeSpec();
}

Qt::TimeSpec CalendarEvent::startTimeSpec() const
{
    return toTimeSpec(mManager->getEvent(mUniqueId, mRecurrenceId).startTime);
}

Qt::TimeSpec CalendarEvent::endTimeSpec() const
{
    return toTimeSpec(mManager->getEvent(mUniqueId, mRecurrenceId).endTime);
}

QString CalendarEvent::startTimeZone() const
{
    return QString::fromLatin1(mManager->getEvent(mUniqueId, mRecurrenceId).startTime.timeZone().id());
}

QString CalendarEvent::endTimeZone() const
{
    return QString::fromLatin1(mManager->getEvent(mUniqueId, mRecurrenceId).endTime.timeZone().id());
}

bool CalendarEvent::allDay() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).allDay;
}

CalendarEvent::Recur CalendarEvent::recur() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).recur;
}

QDateTime CalendarEvent::recurEndDate() const
{
    return QDate(mManager->getEvent(mUniqueId, mRecurrenceId).recurEndDate).startOfDay();
}

bool CalendarEvent::hasRecurEndDate() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).recurEndDate.isValid();
}

CalendarEvent::Days CalendarEvent::recurWeeklyDays() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).recurWeeklyDays;
}

int CalendarEvent::reminder() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).reminder;
}

QDateTime CalendarEvent::reminderDateTime() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).reminderDateTime;
}

QString CalendarEvent::uniqueId() const
{
    return mUniqueId;
}

QString CalendarEvent::color() const
{
    return mManager->getNotebookColor(mManager->getEvent(mUniqueId, mRecurrenceId).calendarUid);
}

bool CalendarEvent::readOnly() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).readOnly;
}

QString CalendarEvent::calendarUid() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).calendarUid;
}

QString CalendarEvent::location() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).location;
}

CalendarEvent::Secrecy CalendarEvent::secrecy() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).secrecy;
}

CalendarEvent::SyncFailure CalendarEvent::syncFailure() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).syncFailure;
}

CalendarEvent::Response CalendarEvent::ownerStatus() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).ownerStatus;
}

bool CalendarEvent::rsvp() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).rsvp;
}

bool CalendarEvent::externalInvitation() const
{
    return mManager->getEvent(mUniqueId, mRecurrenceId).externalInvitation;
}

bool CalendarEvent::sendResponse(int response)
{
    return mManager->sendResponse(mManager->getEvent(mUniqueId, mRecurrenceId), (Response)response);
}

void CalendarEvent::deleteEvent()
{
    mManager->deleteEvent(mUniqueId, mRecurrenceId, QDateTime());
    mManager->save();
}

QDateTime CalendarEvent::recurrenceId() const
{
    return mRecurrenceId;
}

QString CalendarEvent::recurrenceIdString() const
{
    if (mRecurrenceId.isValid()) {
        return CalendarUtils::recurrenceIdToString(mRecurrenceId);
    } else {
        return QString();
    }
}

// Returns the event as a iCalendar string
QString CalendarEvent::iCalendar(const QString &prodId) const
{
    if (mUniqueId.isEmpty()) {
        qWarning() << "Event has no uid, returning empty iCalendar string."
                   << "Save event before calling this function";
        return QString();
    }

    return mManager->convertEventToICalendarSync(mUniqueId, prodId);
}

void CalendarEvent::notebookColorChanged(QString notebookUid)
{
    if (mManager->getEvent(mUniqueId, mRecurrenceId).calendarUid == notebookUid)
        emit colorChanged();
}

void CalendarEvent::eventUidChanged(QString oldUid, QString newUid)
{
    if (mUniqueId == oldUid) {
        mUniqueId = newUid;
        emit uniqueIdChanged();
        // Event uid changes when the event is moved between notebooks, calendar uid has changed
        emit calendarUidChanged();
    }
}
