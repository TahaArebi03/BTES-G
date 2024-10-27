#pragma once
class Booking
{
private:
	int bookingId;
	int	eventId;
	int	userId;
	int	numberOfTicket;
	int	seatTicket;
	int	priceTicket;
	int	totelOfTicketByEventId;
	int	status;
public:
	void confirmBooking();
	void cancelBooking();
	void addBooking();
	void isBookingAvailable();
	void selectSeat();
};

