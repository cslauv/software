from Tkinter import *
import serial
import thread

#{forward:0,backward:1,left:2,right:3,submerge:4,emerge:5}

arduino = serial.Serial('/dev/ttyACM0',9600)
input_count = 0

pwms_dictionary = {"forward":0,"backward":1,"left":2,"right":3,"submerge":4,"emerge":5}

top = Tk()
top.configure(bg="#353839")
top.wm_title("CSLAUV Testing Utility")

def send_command(index,direction,pwm):
	arduino.write(str(index) + ":" + str(pwms_dictionary[direction]) + ":" + str(pwm))

def create_frame(thrusterName, r, c):
	thrusterFrame = Frame(top,bg="#353839")
        label = Label(thrusterFrame, text = thrusterName, justify="center",fg = "#ffd700",bg="#353839", font=("Courier",14))
        label.grid(row=r, column=c, padx = 30, pady = 10)

	var = StringVar(thrusterFrame)
	pwm_option = OptionMenu(thrusterFrame,var,"forward", "backward", "left", "right", "submerge", "emerge")
	pwm_option.config(width =20)
	pwm_option.grid(row = r, column = c+1)
	var.set("forward") # initial value

	entry = Entry(thrusterFrame,bd=5, justify=CENTER)
	entry.grid(row=r,column= c+2)
	entry.insert(END, 1500)


	button = Button(master=thrusterFrame, text='UPDATE', command= lambda: send_command(r - 3,var.get(),entry.get()))
	button.grid(row=r,column= c+3)

	thrusterFrame.grid(row = r, column=c)

console = Frame(top,bg="#000000")
canvas=Canvas(console,bg="#000000", width = 685,height = 230)

def create_console():
	try:
   		thread.start_new_thread(update_console,())
	except:
   		print "Error: unable to start thread"

def myfunction(event):
    canvas.configure(scrollregion=canvas.bbox("all"),width=685,height=230)

def update_console():
	#console = Frame(top,bg="#000000")
	console.grid(row=12,column=0,pady=25)
	console.configure(width = 725,height = 200)

	#canvas=Canvas(console,bg="#000000" )
	frame=Frame(canvas, bg="#000000")
	myscrollbar=Scrollbar(console,orient=VERTICAL,command=canvas.yview)
	canvas.configure(yscrollcommand=myscrollbar.set)

	myscrollbar.pack(side="right",fill="y")
	canvas.pack(side="left")
	canvas.create_window((0,0),window=frame,anchor='nw')
	frame.bind("<Configure>",myfunction)

	count = 0
	while True:
		canvas.yview_moveto(1.0)
		data = arduino.readline()
		if data:
        		label = Label(frame, text = data.strip(),justify="left",fg = "#ffd700",bg="#000000", font=("Courier",10),wraplength=700)
        		label.grid(row=count, column=0, padx = 0, pady = 0,sticky="w")
			canvas.yview_moveto(1.0)
			count += 1


label = Label(top, text = "CSLAUV TESTING UTILITY", justify="center",fg = "#F8F8FF",bg="#353839", font=("Courier",20,"bold"))
label.grid(row=0, column=0, pady = 20)

create_frame("Thruster 1",4,0)
create_frame("Thruster 2",5,0)
create_frame("Thruster 3",6,0)
create_frame("Thruster 4",7,0)
create_frame("Thruster 5",8,0)
create_frame("Thruster 6",9,0)
create_frame("Thruster 7",10,0)
create_frame("Thruster 8",11,0)

create_console()

top.resizable(width=False, height=False)
top.minsize(width=700, height=700)
top.maxsize(width=700, height=700)
top.mainloop()

