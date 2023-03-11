package bug1;

public class Main {
	public static final int N = 100000;
	public static int N_ITERATIONS = 1000;

	public static void main(String[] args) {
		Thread[] threads = new Thread[4];
		boolean sw = true;
		for (int j = 0; j < N_ITERATIONS; j++) {
			MyThread.value = 0;
			for (int i = 0; i < 4; i++) {
				threads[i] = new Thread(new MyThread());
				threads[i].start();
			}
			for (int i = 0; i < 4; i++) {
				try {
					threads[i].join();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			if (MyThread.value != 4 * 3 * N) {
				System.out.println("i was different than " + 4 * 3 * N + " it is " + MyThread.value);
				sw = false;
			}
		}
		if (sw) {
			System.out.println("Something is wrong. The value is always correct.");
		}
	}
}
