Mulai

// =========================
// 1. Siapkan data hotel dan kamar
// =========================
Tentukan daftarHotel[4] dengan {nama, alamat, fasilitas, kamar[3]}   // kamar: Standard / Deluxe / Suite
    // Hotel Mexolie, Hotel Grand Kolopaking, Hotel Putra, Hotel Trio Azana Style
Tentukan akun {nama, email, username, password}
Tentukan pesanan[3] dengan {nama, harga, jumlah}
Tentukan sudahDipilih[3] = false
Tentukan daftarBank = {BCA, BRI, Mandiri}
Tentukan daftarEwallet = {OVO, GoPay, Dana}
BATAS_KAMAR = 100                              // kuota total kamar per transaksi
namaAplikasi = "QuattroHotel"
lokasi = "Kebumen, Jawa Tengah"
jumlahItem = 0
totalKamar = 0
statusPembayaran = "BELUM LUNAS"

// =========================
// 2. Pilihan akun
// =========================
Tampilkan "1. Registrasi"
Tampilkan "2. Login"
Ulangi
    Input pilihanAkun

    IF pilihanAkun = 1 atau pilihanAkun = 2
        pilihanValid = true
    ELSE
        Tampilkan "Silakan memilih antara 1 (Registrasi) atau 2 (Login)!"
        pilihanValid = false
Sampai pilihanValid = true

IF pilihanAkun = 2
    Tampilkan "Anda belum memiliki akun, silakan registrasi terlebih dahulu."

// =========================
// 3. Registrasi akun
// =========================
Ulangi
    Tampilkan "Nama: "
    Input akun.nama

    IF akun.nama tidak kosong
        namaValid = true
    ELSE
        Tampilkan "Nama wajib diisi!"
        namaValid = false
Sampai namaValid = true

Ulangi
    Tampilkan "Email: "
    Input akun.email

    IF email berakhiran "@gmail.com" dan bagian depan tidak ada spasi atau "@"
        emailValid = true
    ELSE
        Tampilkan "Email tidak valid, format nama@gmail.com"
        emailValid = false
Sampai emailValid = true

Ulangi
    Tampilkan "Username: "
    Input akun.username

    IF username tidak kosong dan hanya huruf
        usernameValid = true
    ELSE
        Tampilkan "Username wajib diisi dan hanya huruf!"
        usernameValid = false
Sampai usernameValid = true

Ulangi
    Tampilkan "Password (minimal 8 karakter): "
    Input akun.password (tampil bintang *)

    IF password kosong
        Tampilkan "Password wajib diisi!"
        passwordValid = false
    ELSE IF panjang password < 8
        Tampilkan "Password minimal 8 karakter!"
        passwordValid = false
    ELSE
        passwordValid = true
Sampai passwordValid = true

Tampilkan "Registrasi berhasil!"

// =========================
// 4. Login
// =========================
Ulangi
    Input userLogin
    Input passLogin (tampil bintang *)

    IF userLogin = akun.username dan passLogin = akun.password
        loginBerhasil = true
    ELSE
        Tampilkan "Username atau password salah, coba lagi."
        loginBerhasil = false
Sampai loginBerhasil = true

Tampilkan "Selamat " + akun.nama + ", Anda telah login!"
Tampilkan "Selamat datang di aplikasi " + namaAplikasi
Tampilkan "Lokasi hotel: " + lokasi

// =========================
// 5. Tampilkan daftar hotel
// =========================
Tampilkan "Daftar Hotel"
Untuk setiap hotel di daftarHotel
    Tampilkan nomor, nama, alamat, fasilitas

// =========================
// 6. Input pilihan hotel
// =========================
Ulangi
    Tampilkan "Pilih hotel (1-4): "
    Input pilihanHotel

    IF pilihanHotel >= 1 dan pilihanHotel <= 4
        hotelValid = true
    ELSE
        Tampilkan "Silakan pilih hotel dari pilihan yang tersedia!"
        hotelValid = false
Sampai hotelValid = true

hotelPilihan = daftarHotel[pilihanHotel - 1]
Tampilkan "Anda memilih " + hotelPilihan.nama

// =========================
// 7. Input tanggal check-in
// =========================
tanggalSekarang = ambilTanggalSekarang()
Tampilkan 7 pilihan tanggal (hari ini sampai 6 hari ke depan)
Ulangi
    Input pilihIn

    IF pilihIn >= 1 dan pilihIn <= 7
        checkInValid = true
    ELSE
        Tampilkan "Silakan pilih tanggal check-in dengan angka yang disediakan!"
        checkInValid = false
Sampai checkInValid = true

tanggalCheckIn = tanggalSekarang + (pilihIn - 1) hari

// =========================
// 8. Input tanggal check-out
// =========================
Tampilkan 7 pilihan tanggal check-out beserta jumlah malam
Ulangi
    Input jumlahMalam

    IF jumlahMalam >= 1 dan jumlahMalam <= 7
        checkOutValid = true
    ELSE
        Tampilkan "Silakan pilih tanggal check-out dengan angka yang disediakan!"
        checkOutValid = false
Sampai checkOutValid = true

tanggalCheckOut = tanggalCheckIn + jumlahMalam hari

// =========================
// 9. Pilih kamar
// =========================
lagi = true
Selama lagi = true
    sisaKamar = BATAS_KAMAR - totalKamar
    Tampilkan daftar kamar hotelPilihan (nama, harga per malam, fasilitas)
    Tampilkan "Sisa kuota kamar yang bisa dipesan: " + sisaKamar
    Untuk setiap kamar
        IF sudahDipilih[i] = true
            Tampilkan "(sudah dipilih)"

    Ulangi
        Input pilihanKamar

        IF pilihanKamar < 1 atau pilihanKamar > 3
            Tampilkan "Silakan pilih kamar sesuai angka yang disediakan!"
            kamarValid = false
        ELSE IF sudahDipilih[pilihanKamar - 1] = true
            Tampilkan "Anda sudah memesan kamar ini!"
            kamarValid = false
        ELSE
            kamarValid = true
    Sampai kamarValid = true

    Ulangi
        Input jumlahKamar                      // batas: 1 sampai sisaKamar

        IF jumlahKamar >= 1 dan jumlahKamar <= sisaKamar
            jumlahValid = true
        ELSE
            Tampilkan "Silakan pilih jumlah kamar antara 1 sampai " + sisaKamar + "!"
            jumlahValid = false
    Sampai jumlahValid = true

    Simpan nama, harga, jumlahKamar ke pesanan[jumlahItem]
    sudahDipilih[pilihanKamar - 1] = true
    jumlahItem = jumlahItem + 1
    totalKamar = totalKamar + jumlahKamar

    IF totalKamar >= BATAS_KAMAR
        Tampilkan "Pesanan anda sudah memenuhi batas 100 kamar yang bisa dipesan."
        lagi = false                           // langsung ke rincian harga
    ELSE IF jumlahItem >= 3
        Tampilkan "Semua kategori kamar sudah dipilih."
        lagi = false                           // langsung ke rincian harga
    ELSE
        Ulangi
            Tampilkan "Ingin memilih kategori kamar lain? (y/n)"
            Input jawab

            IF jawab = "y" atau "Y"
                lagi = true
                jawabValid = true
            ELSE IF jawab = "n" atau "N"
                lagi = false
                jawabValid = true
            ELSE
                Tampilkan "Silakan ketik y atau n!"
                jawabValid = false
        Sampai jawabValid = true

// =========================
// 10. Hitung biaya menginap
// =========================
totalHarga = 0
Untuk setiap item di pesanan
    subtotal = harga * jumlah * jumlahMalam
    totalHarga = totalHarga + subtotal

// =========================
// 11. Tampilkan rincian harga
// =========================
Tampilkan nama hotel
Tampilkan jumlahMalam
Untuk setiap item di pesanan
    Tampilkan nama kamar, harga, jumlah, subtotal
Tampilkan totalKamar
Tampilkan totalHarga

// =========================
// 12. Pilih metode dan bayar
// =========================
Selama statusPembayaran bukan "LUNAS"
    Ulangi
        Tampilkan "Metode pembayaran: 1. Transfer Bank | 2. E-Wallet"
        Input metode

        IF metode = 1 atau metode = 2
            metodeValid = true
        ELSE
            Tampilkan "Silakan memilih metode pembayaran yang disediakan!"
            metodeValid = false
    Sampai metodeValid = true

    IF metode = 1                              // Transfer Bank
        Tampilkan daftarBank
        Ulangi
            Input pilihBank

            IF pilihBank >= 1 dan pilihBank <= 3
                bankValid = true
            ELSE
                Tampilkan "Silakan memilih metode pembayaran yang disediakan!"
                bankValid = false
        Sampai bankValid = true
        metodeBayar = "Transfer Bank - " + daftarBank[pilihBank - 1]
    ELSE                                       // E-Wallet
        Tampilkan daftarEwallet
        Ulangi
            Input pilihEwallet

            IF pilihEwallet >= 1 dan pilihEwallet <= 3
                ewalletValid = true
            ELSE
                Tampilkan "Silakan memilih metode pembayaran yang disediakan!"
                ewalletValid = false
        Sampai ewalletValid = true
        metodeBayar = "E-Wallet - " + daftarEwallet[pilihEwallet - 1]

    Tampilkan "Silakan melakukan pembayaran melalui " + metodeBayar
    Tampilkan totalHarga
    Tampilkan "(Harus dibayar dengan nominal pas)"
    Input nominalBayar (titik pemisah muncul otomatis)

    IF nominalBayar = totalHarga               // uang pas
        statusPembayaran = "LUNAS"
        Tampilkan "Pembayaran berhasil!"
    ELSE
        Tampilkan "Nominal pembayaran tidak sesuai! Silakan ulangi."
        // kembali ke pilih metode pembayaran

// =========================
// 13. Cetak konfirmasi booking
// =========================
Tampilkan "KONFIRMASI BOOKING"
Tampilkan akun.nama
Tampilkan nama hotel
Tampilkan alamat hotel
Tampilkan tanggalCheckIn
Tampilkan tanggalCheckOut
Tampilkan jumlahMalam
Untuk setiap item di pesanan
    Tampilkan nama kamar dan jumlah
Tampilkan metodeBayar
Tampilkan totalHarga
Tampilkan statusPembayaran
Tampilkan "Yeayyy! Booking berhasil dilakukan!"

// =========================
// 14. Simpan bukti booking
// =========================
Buka file "booking.txt" mode tambah

IF file berhasil dibuka
    Tulis "BUKTI BOOKING HOTEL"
    Tulis nama pelanggan, nama hotel, lokasi
    Tulis check-in, check-out, jumlah malam
    Untuk setiap item di pesanan
        Tulis nama kamar dan jumlah
    Tulis metode bayar, total harga, status
    Tulis "Booking berhasil dilakukan!"
    Tutup file
    Tampilkan "Bukti booking berhasil disimpan di 'booking.txt'"
ELSE
    Tampilkan "Gagal menyimpan bukti booking!"

Tampilkan "Terima kasih telah melakukan pemesanan di QuattroHotel"

Selesai